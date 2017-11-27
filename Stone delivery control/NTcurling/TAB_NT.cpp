// TAB_NT.cpp : implementation file
//

#include "stdafx.h"
#include "NTcurling.h"
#include "TAB_NT.h"


// CTAB_NT dialog

IMPLEMENT_DYNAMIC(CTAB_NT, CDialog)

CTAB_NT::CTAB_NT(CWnd* pParent /*=NULL*/)
	: CDialog(CTAB_NT::IDD, pParent)
	, m_dJoystic_UP_Down(0)
	, m_dJoystic_Left_Right(0)
	, m_iJoystic_Button(0)
	, m_iStoneVelocity(0)
	, m_dStoneELMO_Current(0)
	, m_dGripperPose(0)
	, m_dGripperELMO_Current(0)
	, m_dLiftPose1(0)
	, m_dLiftPose2(0)
{

}

CTAB_NT::~CTAB_NT()
{
}

void CTAB_NT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dJoystic_UP_Down);
	DDX_Text(pDX, IDC_EDIT2, m_dJoystic_Left_Right);
	DDX_Text(pDX, IDC_EDIT5, m_iJoystic_Button);
	DDX_Text(pDX, IDC_EDIT6, m_iStoneVelocity);
	DDX_Text(pDX, IDC_EDIT7, m_dStoneELMO_Current);
	DDX_Text(pDX, IDC_EDIT8, m_dGripperPose);
	DDX_Text(pDX, IDC_EDIT9, m_dGripperELMO_Current);
	DDX_Text(pDX, IDC_EDIT10, m_dLiftPose1);
	DDX_Text(pDX, IDC_EDIT11, m_dLiftPose2);
}


BEGIN_MESSAGE_MAP(CTAB_NT, CDialog)

	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT8, &CTAB_NT::OnEnChangeEdit8)
	ON_EN_CHANGE(IDC_EDIT9, &CTAB_NT::OnEnChangeEdit9)
END_MESSAGE_MAP()


BOOL CTAB_NT::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	SetTimer(2,16,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTAB_NT::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 2)
	{
		
		UpdateData(FALSE);

	}
	CDialog::OnTimer(nIDEvent);
}
void CTAB_NT::OnEnChangeEdit8()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CTAB_NT::OnEnChangeEdit9()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
