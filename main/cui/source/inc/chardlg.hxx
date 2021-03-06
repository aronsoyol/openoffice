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


#ifndef _SVX_CHARDLG_HXX
#define _SVX_CHARDLG_HXX

// include ---------------------------------------------------------------

#include <svtools/ctrlbox.hxx>
#include <svtools/stdctrl.hxx>
#include <sfx2/tabdlg.hxx>
#include <svx/fntctrl.hxx>
#include <svx/checklbx.hxx>
#include <svx/langbox.hxx>

// forward ---------------------------------------------------------------

class SvxFontListItem;
class FontList;

// -----------------------------------------------------------------------

#define DISABLE_CASEMAP				((sal_uInt16)0x0001)
#define DISABLE_WORDLINE			((sal_uInt16)0x0002)
#define DISABLE_BLINK				((sal_uInt16)0x0004)
#define DISABLE_UNDERLINE_COLOR		((sal_uInt16)0x0008)

#define DISABLE_LANGUAGE			((sal_uInt16)0x0010)
#define DISABLE_HIDE_LANGUAGE		((sal_uInt16)0x0020)

// class SvxCharBasePage -------------------------------------------------

class SvxCharBasePage : public SfxTabPage
{
protected:
	SvxFontPrevWindow	m_aPreviewWin;
	FixedInfo			m_aFontTypeFT;

	sal_Bool				m_bPreviewBackgroundToCharacter;

						SvxCharBasePage( Window* pParent, const ResId& rResIdTabPage, const SfxItemSet&,
											sal_uInt16 nResIdPrewievWin, sal_uInt16 nResIdFontTypeFT );
	virtual				~SvxCharBasePage();

	void				SetPrevFontSize( const SfxItemSet& rSet, sal_uInt16 nSlot, SvxFont& rFont );
	void				SetPrevFont( const SfxItemSet& rSet, sal_uInt16 nSlot, SvxFont& rFont );
	void				SetPrevFontStyle( const SfxItemSet& rSet, sal_uInt16 nSlotPosture, sal_uInt16 nSlotWeight, SvxFont& rFont ); // posture/weight
	void				SetPrevFontWidthScale( const SfxItemSet& rSet );

	void				SetPrevFontEscapement( sal_uInt8 nProp, sal_uInt8 nEscProp, short nEsc );

	inline SvxFont&		GetPreviewFont();
	inline SvxFont&		GetPreviewCJKFont();
	inline SvxFont&		GetPreviewCTLFont();

public:
    using SfxTabPage::ActivatePage;
    using SfxTabPage::DeactivatePage;
    
    virtual void        ActivatePage( const SfxItemSet& rSet );
//    virtual int         DeactivatePage( SfxItemSet* pSet = 0 );

};

// class SvxCharNamePage -------------------------------------------------

struct SvxCharNamePage_Impl;

class SvxCharNamePage : public SvxCharBasePage
{

private:
    FixedLine*          m_pWestLine;
    FixedText*          m_pWestFontNameFT;
    FontNameBox*        m_pWestFontNameLB;
    FixedText*          m_pWestFontStyleFT;
    FontStyleBox*       m_pWestFontStyleLB;
    FixedText*          m_pWestFontSizeFT;
    FontSizeBox*        m_pWestFontSizeLB;
    FixedText*          m_pWestFontLanguageFT;
    SvxLanguageBox*     m_pWestFontLanguageLB;

    FixedLine*          m_pEastLine;
    FixedText*          m_pEastFontNameFT;
    FontNameBox*        m_pEastFontNameLB;
    FixedText*          m_pEastFontStyleFT;
    FontStyleBox*       m_pEastFontStyleLB;
    FixedText*          m_pEastFontSizeFT;
    FontSizeBox*        m_pEastFontSizeLB;
    FixedText*          m_pEastFontLanguageFT;
    SvxLanguageBox*     m_pEastFontLanguageLB;

    FixedLine*          m_pCTLLine;
    FixedText*          m_pCTLFontNameFT;
    FontNameBox*        m_pCTLFontNameLB;
    FixedText*          m_pCTLFontStyleFT;
    FontStyleBox*       m_pCTLFontStyleLB;
    FixedText*          m_pCTLFontSizeFT;
    FontSizeBox*        m_pCTLFontSizeLB;
    FixedText*          m_pCTLFontLanguageFT;
    SvxLanguageBox*     m_pCTLFontLanguageLB;

    FixedLine*          m_pColorFL;
    FixedText*          m_pColorFT;
    ColorListBox*       m_pColorLB;

	SvxCharNamePage_Impl*	m_pImpl;

						SvxCharNamePage( Window* pParent, const SfxItemSet& rSet );

	void				Initialize();
	const FontList*		GetFontList() const;
	void                UpdatePreview_Impl();
	void                FillStyleBox_Impl( const FontNameBox* rBox );
	void                FillSizeBox_Impl( const FontNameBox* rBox );

	enum LanguageGroup
	{
		/** Language for western text.
		 */
		Western = 0,

		/** Language for asian text.
		 */
		Asian,

		/** Language for ctl text.
		 */
		Ctl
	};

	void				Reset_Impl( const SfxItemSet& rSet, LanguageGroup eLangGrp );
	sal_Bool				FillItemSet_Impl( SfxItemSet& rSet, LanguageGroup eLangGrp );
	void				ResetColor_Impl( const SfxItemSet& rSet );

	DECL_LINK( 			UpdateHdl_Impl, Timer* );
	DECL_LINK(			FontModifyHdl_Impl, void* );
	DECL_LINK( 			ColorBoxSelectHdl_Impl, ColorListBox* );

public:
    using SfxTabPage::ActivatePage;
    using SfxTabPage::DeactivatePage;
    
    virtual void        ActivatePage( const SfxItemSet& rSet );
	virtual int         DeactivatePage( SfxItemSet* pSet = 0 );

public:
						~SvxCharNamePage();

	static SfxTabPage*  Create( Window* pParent, const SfxItemSet& rSet );
	static sal_uInt16*      GetRanges();

	virtual void        Reset( const SfxItemSet& rSet );
	virtual sal_Bool        FillItemSet( SfxItemSet& rSet );

	void                SetFontList( const SvxFontListItem& rItem );
	void                EnableRelativeMode();
	void				EnableSearchMode();
    //                  the writer uses SID_ATTR_BRUSH as font background
    void                SetPreviewBackgroundToCharacter();

	void                DisableControls( sal_uInt16 nDisable );
	virtual void		PageCreated (SfxAllItemSet aSet); 
};

// class SvxCharEffectsPage ----------------------------------------------

class SvxCharEffectsPage : public SvxCharBasePage
{

private:
	FixedText			m_aFontColorFT;
	ColorListBox		m_aFontColorLB;

	FixedText			m_aEffectsFT;
	SvxCheckListBox		m_aEffectsLB;

	ListBox				m_aEffects2LB;

    FixedText           m_aReliefFT;
    ListBox             m_aReliefLB;

    TriStateBox         m_aOutlineBtn;
	TriStateBox			m_aShadowBtn;
	TriStateBox			m_aBlinkingBtn;
    TriStateBox         m_aHiddenBtn;

	FixedLine			m_aVerticalLine;

	FixedText			m_aOverlineFT;
	ListBox				m_aOverlineLB;
	FixedText			m_aOverlineColorFT;
	ColorListBox		m_aOverlineColorLB;

	FixedText			m_aStrikeoutFT;
	ListBox				m_aStrikeoutLB;

	FixedText			m_aUnderlineFT;
	ListBox				m_aUnderlineLB;
	FixedText			m_aUnderlineColorFT;
	ColorListBox		m_aUnderlineColorLB;

	CheckBox			m_aIndividualWordsBtn;

	FixedLine			m_aAsianLine;

	FixedText			m_aEmphasisFT;
	ListBox				m_aEmphasisLB;

	FixedText			m_aPositionFT;
	ListBox				m_aPositionLB;

	sal_uInt16				m_nHtmlMode;

	String				m_aTransparentColorName;

						SvxCharEffectsPage( Window* pParent, const SfxItemSet& rSet );

	void				Initialize();
	void                UpdatePreview_Impl();
	void                SetCaseMap_Impl( SvxCaseMap eCaseMap );
	void				ResetColor_Impl( const SfxItemSet& rSet );
	sal_Bool				FillItemSetColor_Impl( SfxItemSet& rSet );

	DECL_LINK(			SelectHdl_Impl, ListBox* );
    DECL_LINK(          CbClickHdl_Impl, CheckBox* );
    DECL_LINK(          TristClickHdl_Impl, TriStateBox* );
	DECL_LINK(			UpdatePreview_Impl, ListBox* );
	DECL_LINK( 			ColorBoxSelectHdl_Impl, ColorListBox* );

public:
//    using SfxTabPage::ActivatePage;
    using SfxTabPage::DeactivatePage;

//    virtual void        ActivatePage( const SfxItemSet& rSet );
    virtual int         DeactivatePage( SfxItemSet* pSet = 0 );

public:
	static SfxTabPage*  Create( Window* pParent, const SfxItemSet& rSet );
	static sal_uInt16*      GetRanges();

	virtual void        Reset( const SfxItemSet& rSet );
	virtual sal_Bool        FillItemSet( SfxItemSet& rSet );

	void                DisableControls( sal_uInt16 nDisable );
	void                EnableFlash();
    //                  the writer uses SID_ATTR_BRUSH as font background
    void                SetPreviewBackgroundToCharacter();
	virtual void		PageCreated (SfxAllItemSet aSet); 
};

// class SvxCharPositionPage ---------------------------------------------

class SvxCharPositionPage : public SvxCharBasePage
{

private:
	FixedLine			m_aPositionLine;
	RadioButton			m_aHighPosBtn;
	RadioButton			m_aNormalPosBtn;
	RadioButton			m_aLowPosBtn;
	FixedText			m_aHighLowFT;
	MetricField			m_aHighLowEdit;
	CheckBox			m_aHighLowRB;
	FixedText			m_aFontSizeFT;
	MetricField			m_aFontSizeEdit;
	FixedLine			m_aRotationScalingFL;
	FixedLine			m_aScalingFL;
	RadioButton			m_a0degRB;
	RadioButton			m_a90degRB;
	RadioButton			m_a270degRB;
	CheckBox			m_aFitToLineCB;
	FixedText			m_aScaleWidthFT;
	MetricField			m_aScaleWidthMF;

	FixedLine			m_aKerningLine;
	ListBox				m_aKerningLB;
	FixedText			m_aKerningFT;
	MetricField			m_aKerningEdit;
	CheckBox			m_aPairKerningBtn;

	short               m_nSuperEsc;
	short               m_nSubEsc;

	sal_uInt16				m_nScaleWidthItemSetVal;
	sal_uInt16				m_nScaleWidthInitialVal;

	sal_uInt8                m_nSuperProp;
	sal_uInt8                m_nSubProp;

						SvxCharPositionPage( Window* pParent, const SfxItemSet& rSet );

	void				Initialize();
	void                UpdatePreview_Impl( sal_uInt8 nProp, sal_uInt8 nEscProp, short nEsc );
	void                SetEscapement_Impl( sal_uInt16 nEsc );

	DECL_LINK(			PositionHdl_Impl, RadioButton* );
	DECL_LINK(			RotationHdl_Impl, RadioButton* );
	DECL_LINK(			FontModifyHdl_Impl, MetricField* );
	DECL_LINK(			AutoPositionHdl_Impl, CheckBox* );
	DECL_LINK(			FitToLineHdl_Impl, CheckBox* );
	DECL_LINK(			KerningSelectHdl_Impl, ListBox* );
	DECL_LINK(			KerningModifyHdl_Impl, MetricField* );
	DECL_LINK( 			PairKerningHdl_Impl, CheckBox* );
	DECL_LINK(			LoseFocusHdl_Impl, MetricField* );
	DECL_LINK(			ScaleWidthModifyHdl_Impl, MetricField* );

public:
    using SfxTabPage::ActivatePage;
    using SfxTabPage::DeactivatePage;
    
    virtual int         DeactivatePage( SfxItemSet* pSet = 0 );
    virtual void        ActivatePage( const SfxItemSet& rSet );

public:
	static SfxTabPage*  Create( Window* pParent, const SfxItemSet& rSet );
	static sal_uInt16*      GetRanges();

	virtual void        Reset( const SfxItemSet& rSet );
	virtual sal_Bool        FillItemSet( SfxItemSet& rSet );
	virtual void        FillUserData();
    //                  the writer uses SID_ATTR_BRUSH as font background
    void                SetPreviewBackgroundToCharacter();
	virtual void		PageCreated (SfxAllItemSet aSet); 
};

// class SvxCharTwoLinesPage ---------------------------------------------

class SvxCharTwoLinesPage : public SvxCharBasePage
{
private:
	FixedLine			m_aSwitchOnLine;
	CheckBox			m_aTwoLinesBtn;

	FixedLine			m_aEncloseLine;
	FixedText			m_aStartBracketFT;
	ListBox				m_aStartBracketLB;
	FixedText			m_aEndBracketFT;
	ListBox				m_aEndBracketLB;

	sal_uInt16				m_nStartBracketPosition;
	sal_uInt16				m_nEndBracketPosition;

						SvxCharTwoLinesPage( Window* pParent, const SfxItemSet& rSet );

	void				UpdatePreview_Impl();
	void				Initialize();
	void				SelectCharacter( ListBox* pBox );
	void				SetBracket( sal_Unicode cBracket, sal_Bool bStart );

	DECL_LINK( 			TwoLinesHdl_Impl, CheckBox* );
	DECL_LINK(			CharacterMapHdl_Impl, ListBox* );

public:
    using SfxTabPage::ActivatePage;
    using SfxTabPage::DeactivatePage;
    
    virtual void        ActivatePage( const SfxItemSet& rSet );
	virtual int         DeactivatePage( SfxItemSet* pSet = 0 );

public:
	static SfxTabPage*  Create( Window* pParent, const SfxItemSet& rSet );
	static sal_uInt16*      GetRanges();

	virtual void        Reset( const SfxItemSet& rSet );
	virtual sal_Bool        FillItemSet( SfxItemSet& rSet );
    //                  the writer uses SID_ATTR_BRUSH as font background
    void                SetPreviewBackgroundToCharacter();
	virtual void		PageCreated (SfxAllItemSet aSet); 
};

#endif // #ifndef _SVX_CHARDLG_HXX

