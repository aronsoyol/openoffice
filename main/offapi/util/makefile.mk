#**************************************************************
#  
#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.
#  
#**************************************************************



PRJ=..
PRJPCH=

PRJNAME=offapi
TARGET=offapi_db


# --- Settings -----------------------------------------------------

.INCLUDE :  makefile.pmk

# ------------------------------------------------------------------
UNOIDLDBFILES= \
    $(UCR)$/cssauth.db \
    $(UCR)$/cssawt.db \
    $(UCR)$/cssawttree.db \
    $(UCR)$/cssawtgrid.db \
    $(UCR)$/cssawttab.db \
    $(UCR)$/csschart.db \
    $(UCR)$/csschart2.db \
    $(UCR)$/csschart2data.db \
    $(UCR)$/cssconfiguration.db \
    $(UCR)$/csscbootstrap.db\
    $(UCR)$/csscbackend.db\
    $(UCR)$/csscbxml.db\
    $(UCR)$/cssdatatransfer.db \
    $(UCR)$/cssdeployment.db \
    $(UCR)$/cssdeploymenttest.db \
    $(UCR)$/cssdeploymentui.db \
    $(UCR)$/cssdclipboard.db \
    $(UCR)$/cssddnd.db \
    $(UCR)$/cssdocument.db \
    $(UCR)$/cssdrawing.db \
    $(UCR)$/cssdframework.db \
    $(UCR)$/cssgeometry.db \
    $(UCR)$/cssrendering.db \
    $(UCR)$/cssembed.db \
    $(UCR)$/cssfcontrol.db \
    $(UCR)$/cssfinsp.db \
    $(UCR)$/cssfrunt.db \
    $(UCR)$/cssform.db \
    $(UCR)$/cssxforms.db \
    $(UCR)$/cssfcomp.db \
    $(UCR)$/cssfbinding.db \
    $(UCR)$/cssfvalidation.db \
    $(UCR)$/cssfsubmission.db \
    $(UCR)$/cssformula.db \
    $(UCR)$/cssgraphic.db \
    $(UCR)$/cssinstallation.db \
    $(UCR)$/cssi18n.db \
    $(UCR)$/cssldap.db \
    $(UCR)$/csslinguistic2.db \
    $(UCR)$/csslogging.db \
    $(UCR)$/cssmail.db \
    $(UCR)$/cssmedia.db \
    $(UCR)$/cssmozilla.db \
    $(UCR)$/cssoooimprovement.db \
    $(UCR)$/csspackages.db \
    $(UCR)$/cssmanifest.db \
    $(UCR)$/csszippackage.db \
    $(UCR)$/cssplugin.db \
    $(UCR)$/csspresentation.db \
    $(UCR)$/cssanimations.db \
    $(UCR)$/cssresource.db \
    $(UCR)$/csssax.db \
    $(UCR)$/cssdom.db \
    $(UCR)$/cssxpath.db \
    $(UCR)$/cssdomviews.db \
    $(UCR)$/cssdomevents.db \
    $(UCR)$/cssscanner.db \
    $(UCR)$/cssscript.db \
    $(UCR)$/cssscriptvba.db \
    $(UCR)$/csssdb.db \
    $(UCR)$/csssdbc.db \
    $(UCR)$/csssdbcx.db \
    $(UCR)$/csssdbtools.db \
    $(UCR)$/csssdbapp.db \
    $(UCR)$/csssheet.db \
    $(UCR)$/csssmarttags.db \
    $(UCR)$/cssstyle.db \
    $(UCR)$/csssystem.db\
    $(UCR)$/csstable.db \
    $(UCR)$/csstask.db \
    $(UCR)$/csstext.db \
    $(UCR)$/csstextfield.db \
    $(UCR)$/csstextfielddocinfo.db \
    $(UCR)$/cssfieldmaster.db \
    $(UCR)$/cssucb.db \
    $(UCR)$/cssutil.db \
    $(UCR)$/cssview.db \
    $(UCR)$/csssvg.db \
    $(UCR)$/csssync.db \
    $(UCR)$/csssync2.db \
    $(UCR)$/cssxml.db \
    $(UCR)$/cssxmlinput.db \
    $(UCR)$/csssetup.db \
    $(UCR)$/cssimage.db\
    $(UCR)$/cssuidialogs.db \
    $(UCR)$/cssui.db \
    $(UCR)$/cssframe.db \
    $(UCR)$/cssfstatus.db \
    $(UCR)$/cssgraphic.db \
    $(UCR)$/cssaccessibility.db\
    $(UCR)$/csssprovider.db \
    $(UCR)$/csssbrowse.db \
    $(UCR)$/cssgallery.db \
    $(UCR)$/cssxsd.db \
    $(UCR)$/cssinspection.db \
    $(UCR)$/xsec-security.db \
    $(UCR)$/xsec-crypto.db \
    $(UCR)$/xsec-csax.db \
    $(UCR)$/xsec-sax.db \
    $(UCR)$/xsec-wrapper.db \
    $(UCR)$/cssreport.db \
    $(UCR)$/cssrptins.db \
    $(UCR)$/cssrptmeta.db \
    $(UCR)$/cssoffice.db \
    $(UCR)$/cssrdf.db


REFERENCE_RDB=$(PRJ)$/type_reference$/types.rdb

REGISTRYCHECKFLAG=$(MISC)$/registrycheck.flag

UNOTYPE_STATISTICS=$(MISC)$/unotype_statistics.txt

REGVIEWTOOL=$(SOLARBINDIR)$/regview$(EXECPOST)

# --- Targets ------------------------------------------------------

ALLTAR : $(UCR)$/types.db \
       $(OUT)$/ucrdoc$/types_doc.db \
       $(REGISTRYCHECKFLAG) \
       $(UNOTYPE_STATISTICS)

$(UCR)$/types.db : $(UCR)$/offapi.db $(SOLARBINDIR)$/udkapi.rdb
    @-$(RM) $(REGISTRYCHECKFLAG)
    @$(GNUCOPY) -f $(UCR)$/offapi.db $@
    $(COMMAND_ECHO)$(REGMERGE) $@ / $(SOLARBINDIR)$/udkapi.rdb

$(OUT)$/ucrdoc$/types_doc.db : $(OUT)$/ucrdoc$/offapi_doc.db $(SOLARBINDIR)$/udkapi_doc.rdb
    @-$(RM) $(REGISTRYCHECKFLAG)
    @$(GNUCOPY) -f $(OUT)$/ucrdoc$/offapi_doc.db $@
    $(COMMAND_ECHO)$(REGMERGE) $@ / $(SOLARBINDIR)$/udkapi_doc.rdb

#JSC: The type library has changed, all temporary not checked types are removed
#     and will be check from now on.
# ATTENTION: no special handling for types is allowed.
#
#JSC: i have removed the doc rdb because all type information is already in the
#     types.rdb, even the service and singleton type info. IDL docu isn't checked.
$(REGISTRYCHECKFLAG) : $(UCR)$/types.db $(OUT)$/ucrdoc$/types_doc.db
    $(COMMAND_ECHO)$(REGCOMPARE) -f -t -r1 $(REFERENCE_RDB) -r2 $(UCR)$/types.db \
        && echo > $(REGISTRYCHECKFLAG)

#JSC: new target to prepare some UNO type statistics, the ouput will be later used
#     for versioning of UNO cli type libraries
$(UNOTYPE_STATISTICS) : $(REGISTRYCHECKFLAG)
    $(COMMAND_ECHO)$(AUGMENT_LIBRARY_PATH) $(PERL) checknewapi.pl $(UCR)$/types.db $(REFERENCE_RDB) "$(RSCREVISION)" "$(REGVIEWTOOL)" > $@

.INCLUDE :  target.mk
