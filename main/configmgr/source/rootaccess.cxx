/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#include "precompiled_configmgr.hxx"
#include "sal/config.h"

#include <vector>

#include "com/sun/star/lang/DisposedException.hpp"
#include "com/sun/star/lang/EventObject.hpp"
#include "com/sun/star/lang/WrappedTargetException.hpp"
#include "com/sun/star/uno/Any.hxx"
#include "com/sun/star/uno/Reference.hxx"
#include "com/sun/star/uno/RuntimeException.hpp"
#include "com/sun/star/uno/Type.hxx"
#include "com/sun/star/uno/XInterface.hpp"
#include "com/sun/star/util/ChangesEvent.hpp"
#include "com/sun/star/util/ChangesSet.hpp"
#include "com/sun/star/util/ElementChange.hpp"
#include "com/sun/star/util/XChangesBatch.hpp"
#include "com/sun/star/util/XChangesListener.hpp"
#include "com/sun/star/util/XChangesNotifier.hpp"
#include "comphelper/sequenceasvector.hxx"
#include "cppu/unotype.hxx"
#include "cppuhelper/queryinterface.hxx"
#include "cppuhelper/weak.hxx"
#include "osl/diagnose.h"
#include "osl/mutex.hxx"
#include "rtl/ref.hxx"
#include "rtl/ustring.h"
#include "rtl/ustring.hxx"

#include "broadcaster.hxx"
#include "childaccess.hxx"
#include "components.hxx"
#include "data.hxx"
#include "lock.hxx"
#include "modifications.hxx"
#include "node.hxx"
#include "path.hxx"
#include "rootaccess.hxx"

namespace configmgr {

namespace {

namespace css = com::sun::star;

}

RootAccess::RootAccess(
    Components & components, rtl::OUString const & pathRepresentation,
    rtl::OUString const & locale, bool update):
    Access(components), pathRepresentation_(pathRepresentation),
    locale_(locale), update_(update)
{}

Path RootAccess::getAbsolutePath() {
    getNode();
    return path_;
}

void RootAccess::initBroadcaster(
    Modifications::Node const & modifications, Broadcaster * broadcaster)
{
    OSL_ASSERT(broadcaster != 0);
    comphelper::SequenceAsVector< css::util::ElementChange > changes;
    initBroadcasterAndChanges(
        modifications, broadcaster, changesListeners_.empty() ? 0 : &changes);
    if (!changes.empty()) {
        css::util::ChangesSet set(changes.getAsConstList());
        for (ChangesListeners::iterator i(changesListeners_.begin());
             i != changesListeners_.end(); ++i)
        {
            cppu::OWeakObject* pSource = static_cast< cppu::OWeakObject * >(this);
            css::uno::Reference< css::uno::XInterface > xBase( pSource, css::uno::UNO_QUERY );
            broadcaster->addChangesNotification(
                *i,
                css::util::ChangesEvent(
                    pSource, makeAny( xBase ), set));
        }
    }
}

void RootAccess::acquire() throw () {
    Access::acquire();
}

void RootAccess::release() throw () {
    Access::release();
}

rtl::OUString RootAccess::getAbsolutePathRepresentation() {
    getNode(); // turn pathRepresentation_ into canonic form
    return pathRepresentation_;
}

rtl::OUString RootAccess::getLocale() const {
    return locale_;
}

bool RootAccess::isUpdate() const {
    return update_;
}

RootAccess::~RootAccess() {
    osl::MutexGuard g(lock);
    getComponents().removeRootAccess(this);
}

Path RootAccess::getRelativePath() {
    return Path();
}

rtl::OUString RootAccess::getRelativePathRepresentation() {
    return rtl::OUString();
}

rtl::Reference< Node > RootAccess::getNode() {
    if (!node_.is()) {
        rtl::OUString canonic;
        int finalizedLayer;
        node_ = getComponents().resolvePathRepresentation(
            pathRepresentation_, &canonic, &path_, &finalizedLayer);
        if (!node_.is()) {
            throw css::uno::RuntimeException(
                (rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("cannot find ")) +
                 pathRepresentation_),
                0);
                // RootAccess::queryInterface indirectly calls
                // RootAccess::getNode, so if this RootAccess were passed out in
                // RuntimeException.Context, client code that called
                // queryInterface on it would cause trouble; therefore,
                // RuntimeException.Context is left null here
        }
        pathRepresentation_ = canonic;
        OSL_ASSERT(!path_.empty());
        name_ = path_.back();
        finalized_ = finalizedLayer != Data::NO_LAYER;
    }
    return node_;
}

bool RootAccess::isFinalized() {
    getNode();
    return finalized_;
}

rtl::OUString RootAccess::getNameInternal() {
    getNode();
    return name_;
}

rtl::Reference< RootAccess > RootAccess::getRootAccess() {
    return this;
}

rtl::Reference< Access > RootAccess::getParentAccess() {
    return rtl::Reference< Access >();
}

void RootAccess::addTypes(std::vector< css::uno::Type > * types) const {
    OSL_ASSERT(types != 0);
    types->push_back(cppu::UnoType< css::util::XChangesNotifier >::get());
    types->push_back(cppu::UnoType< css::util::XChangesBatch >::get());
}

void RootAccess::addSupportedServiceNames(
    std::vector< rtl::OUString > * services)
{
    OSL_ASSERT(services != 0);
    services->push_back(
        rtl::OUString(
            RTL_CONSTASCII_USTRINGPARAM(
                "com.sun.star.configuration.AccessRootElement")));
    if (update_) {
        services->push_back(
            rtl::OUString(
                RTL_CONSTASCII_USTRINGPARAM(
                    "com.sun.star.configuration.UpdateRootElement")));
    }
}

void RootAccess::initDisposeBroadcaster(Broadcaster * broadcaster) {
    OSL_ASSERT(broadcaster != 0);
    for (ChangesListeners::iterator i(changesListeners_.begin());
         i != changesListeners_.end(); ++i)
    {
        broadcaster->addDisposeNotification(
            i->get(),
            css::lang::EventObject(static_cast< cppu::OWeakObject * >(this)));
    }
    Access::initDisposeBroadcaster(broadcaster);
}

void RootAccess::clearListeners() throw() {
    changesListeners_.clear();
    Access::clearListeners();
}

css::uno::Any RootAccess::queryInterface(css::uno::Type const & aType)
    throw (css::uno::RuntimeException)
{
    OSL_ASSERT(thisIs(IS_ANY));
    osl::MutexGuard g(lock);
    checkLocalizedPropertyAccess();
    css::uno::Any res(Access::queryInterface(aType));
    if (res.hasValue()) {
        return res;
    }
    res = cppu::queryInterface(
        aType, static_cast< css::util::XChangesNotifier * >(this));
    if (res.hasValue()) {
        return res;
    }
    if (!res.hasValue() && update_) {
        res = cppu::queryInterface(
            aType, static_cast< css::util::XChangesBatch * >(this));
    }
    return res;
}

void RootAccess::addChangesListener(
    css::uno::Reference< css::util::XChangesListener > const & aListener)
    throw (css::uno::RuntimeException)
{
    OSL_ASSERT(thisIs(IS_ANY));
    {
        osl::MutexGuard g(lock);
        checkLocalizedPropertyAccess();
        if (!aListener.is()) {
            throw css::uno::RuntimeException(
                rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("null listener")),
                static_cast< cppu::OWeakObject * >(this));
        }
        if (!isDisposed()) {
            changesListeners_.insert(aListener);
            return;
        }
    }
    try {
        aListener->disposing(
            css::lang::EventObject(static_cast< cppu::OWeakObject * >(this)));
    } catch (css::lang::DisposedException &) {}
}

void RootAccess::removeChangesListener(
    css::uno::Reference< css::util::XChangesListener > const & aListener)
    throw (css::uno::RuntimeException)
{
    OSL_ASSERT(thisIs(IS_ANY));
    osl::MutexGuard g(lock);
    checkLocalizedPropertyAccess();
    ChangesListeners::iterator i(changesListeners_.find(aListener));
    if (i != changesListeners_.end()) {
        changesListeners_.erase(i);
    }
}

void RootAccess::commitChanges()
    throw (css::lang::WrappedTargetException, css::uno::RuntimeException)
{
    OSL_ASSERT(thisIs(IS_UPDATE));
    Broadcaster bc;
    {
        osl::MutexGuard g(lock);
        checkLocalizedPropertyAccess();
        int finalizedLayer;
        Modifications globalMods;
        commitChildChanges(
            ((getComponents().resolvePathRepresentation(
                  pathRepresentation_, 0, 0, &finalizedLayer)
              == node_) &&
             finalizedLayer == Data::NO_LAYER),
            &globalMods);
        getComponents().writeModifications();
        getComponents().initGlobalBroadcaster(globalMods, this, &bc);
    }
    bc.send();
}

sal_Bool RootAccess::hasPendingChanges() throw (css::uno::RuntimeException) {
    OSL_ASSERT(thisIs(IS_UPDATE));
    osl::MutexGuard g(lock);
    checkLocalizedPropertyAccess();
    //TODO: Optimize:
    std::vector< css::util::ElementChange > changes;
    reportChildChanges(&changes);
    return !changes.empty();
}

css::util::ChangesSet RootAccess::getPendingChanges()
    throw (css::uno::RuntimeException)
{
    OSL_ASSERT(thisIs(IS_UPDATE));
    osl::MutexGuard g(lock);
    checkLocalizedPropertyAccess();
    comphelper::SequenceAsVector< css::util::ElementChange > changes;
    reportChildChanges(&changes);
    return changes.getAsConstList();
}

}
