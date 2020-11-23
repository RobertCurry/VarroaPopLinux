#include "cdialog.h"

CPoint::CPoint(uint32_t x, uint32_t y)
{
    NOT_IMPLEMENTED();
}

size_t CWnd::DoModal() {NOT_IMPLEMENTED(); return 0;}
void CWnd::DoDataExchange(CDataExchange*) {NOT_IMPLEMENTED();}
CWnd* CWnd::GetDlgItem(int /*nID*/) const {NOT_IMPLEMENTED(); return nullptr;}
bool CWnd::EnableWindow(bool /*bEnable*/) {NOT_IMPLEMENTED(); return false;}
bool CWnd::UpdateData(bool /*bSaveAndValidate*/) {NOT_IMPLEMENTED(); return false;}
void CWnd::DestroyWindow() {NOT_IMPLEMENTED();}


CDialog::CDialog(){NOT_IMPLEMENTED();}
CDialog::CDialog(int32_t id, CWnd* pParent){NOT_IMPLEMENTED();}
void CDialog::Create(int32_t nIDTemplate){NOT_IMPLEMENTED();}
void CDialog::Create(int32_t nIDTemplate, CWnd* pParentWnd){NOT_IMPLEMENTED();}
bool CDialog::SetWindowText(const CString& text){NOT_IMPLEMENTED(); return false;}
void CDialog::OnOK(){NOT_IMPLEMENTED();}
void CDialog::OnCancel(){NOT_IMPLEMENTED();}

void CProgressCtrl::StepIt(){NOT_IMPLEMENTED();}
