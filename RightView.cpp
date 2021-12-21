
// RightView.cpp : implementation of the CRightView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "3455.h"
#endif

#include "ExcerciseDoc.h"
#include "RightView.h"
#include "LeftView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRightView

IMPLEMENT_DYNCREATE(CRightView, CListView)

BEGIN_MESSAGE_MAP(CRightView, CListView)
	ON_WM_STYLECHANGED()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CRightView::OnNMDblclk)
END_MESSAGE_MAP()

// CRightView construction/destruction

CRightView::CRightView()
{
	// TODO: add construction code here

}

CRightView::~CRightView()
{
}

BOOL CRightView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

void CRightView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
	CListCtrl &ctlList = this->GetListCtrl();

	ctlList.ModifyStyle(NULL,LVS_REPORT);

}

void CRightView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRightView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRightView diagnostics

#ifdef _DEBUG
void CRightView::AssertValid() const
{
	CListView::AssertValid();
}

void CRightView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CExcerciseDoc* CRightView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExcerciseDoc)));
	return (CExcerciseDoc*)m_pDocument;
}
#endif //_DEBUG


// CRightView message handlers
void CRightView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window	
	CListView::OnStyleChanged(nStyleType,lpStyleStruct);	
}


int CRightView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	GetDocument()->pRightView = this;
	return 0;
}


// function responsible for resetting window added by me
void CRightView::resetFiles(void)
{
	CListCtrl &ctlLView = GetListCtrl();
	ctlLView.DeleteAllItems();

	while(ctlLView.DeleteColumn(0))
		;

	UpdateWindow();
}


// Main function responsible for displaying files in right view according to option choosed in left view
void CRightView::displayFiles(LPTSTR Path)
{
	   CListCtrl &ctlRightView = this->GetListCtrl();
	   resetFiles();

	   ctlRightView.InsertColumn(0, L"FILE NAME", LVCFMT_LEFT, 160);
	   ctlRightView.InsertColumn(1, L"FILE SIZE", LVCFMT_LEFT, 40);
	
	   //AfxMessage box PATH
	   WIN32_FIND_DATA FindFileData;
	   HANDLE hFind;

	   int nItem;

	   hFind = FindFirstFile(Path , &FindFileData);
	   int n =0;

	   if(hFind != INVALID_HANDLE_VALUE){
		   do{
			   if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
				   && FindFileData.cFileName != CString(".")
				   && FindFileData.cFileName != CString(".."))
			   {
					++n;
					nItem = ctlRightView.InsertItem(n, L"FILE",2);

					ctlRightView.SetItemText(nItem,0, FindFileData.cFileName);
					long lFsize = FindFileData.nFileSizeLow;
					CString strFSize=L"";
					strFSize.Format(L"%d",lFsize);
					
					ctlRightView.SetItemText(nItem, 1, strFSize.GetBuffer(1));


			   
			   
			   }

		   }while((::WaitForSingleObject(m_hStopEvent, 0) != WAIT_OBJECT_0)
			       && (::FindNextFile(hFind, &FindFileData)));
		   ::FindClose(hFind);
	   }

}


void CRightView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	CExcerciseDoc *pDoc= this->GetDocument();

	CListCtrl &ctlRightView = this->GetListCtrl();

	int nItem = ctlRightView.GetSelectionMark();

	CString string = ctlRightView.GetItemText(nItem,0);

	pDoc->pLeftView->SetFocus();
	pDoc->pLeftView->onRightViewFolderSelected(string, nItem);
	//pDoc->pRightView->displayFiles(string);
}
