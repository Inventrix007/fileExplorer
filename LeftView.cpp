
// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "3455.h"


#include "ExcerciseDoc.h"
#include "LeftView.h"
#include "RightView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CLeftView::OnTvnSelchanged)
END_MESSAGE_MAP()


// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
	CTreeCtrl().ModifyStyle(NULL, TVS_LINESATROOT | TVS_HASLINES | TVS_HASBUTTONS);
}


// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CExcerciseDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExcerciseDoc)));
	return (CExcerciseDoc*)m_pDocument;
}
#endif //_DEBUG


// CLeftView message handlers


int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	GetDocument()->pLeftView = this;
	return 0;
}


// create roots of left tree
void CLeftView::createRoots(void)
{
	GetTreeCtrl().DeleteAllItems();

	CTreeCtrl &ctlDrives = this->GetTreeCtrl();

	HTREEITEM hRoot;

	char *strBuffer = NULL;
	CString strMessage;

	int nPos=0;
	UINT nCount =0;
	CString strDrive = _T("?:\\:");

	DWORD dwDriveList = ::GetLogicalDrives();

	CString cTmp;

	while(dwDriveList){
		if(dwDriveList & 1){
			cTmp = strDrive;
			strDrive.SetAt(0 , 0x41 + nPos);

			strDrive = strDrive.Left(2);
			hRoot = ctlDrives.InsertItem(strDrive,0,1);
		}
		dwDriveList >>=1;
		nPos++;
	}
}


void CLeftView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CExcerciseDoc *pdoc = GetDocument();
	
	CTreeCtrl &ctlFiles = this->GetTreeCtrl();

	HTREEITEM nodeSelected = ctlFiles.GetSelectedItem();

	CString strText = ctlFiles.GetItemText(nodeSelected);

	HTREEITEM parentNode = nodeSelected;

	do{
		parentNode = ctlFiles.GetParentItem(parentNode);

		if(parentNode != NULL)
			strText = ctlFiles.GetItemText(parentNode)+ "\\"+ strText;
	}while(parentNode != NULL);

	CString strSearchPath = strText + "\\*.*";
	pdoc->pRightView->displayFiles(strSearchPath.GetBuffer(1));
	openFolder(strText);
}



void CLeftView::openFolder(CString CStrPath)
{
	CTreeCtrl &ctlFolders = this->GetTreeCtrl();

	HTREEITEM hRoot;
	HTREEITEM hFolder;

	hRoot = ctlFolders.GetSelectedItem();

	HTREEITEM hChild = ctlFolders.GetChildItem(hRoot);

	while(hChild !=0){
		ctlFolders.DeleteItem(hChild);
		hChild = ctlFolders.GetChildItem(hRoot);
	}

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	CStrPath = CStrPath + L"\\*.*";

	hFind = FindFirstFile(CStrPath, &FindFileData);

	if(hFind!= INVALID_HANDLE_VALUE){
		do{
			long lFileSize = FindFileData.nFileSizeLow;

			CString strText = L"";

			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
				&& FindFileData.cFileName != L"."
				&& FindFileData.cFileName != L"..")
			{
				hFolder = ctlFolders.InsertItem(FindFileData.cFileName, 2,3, hRoot);

			}	

		}while(::WaitForSingleObject(m_hStopEvent,0) != WAIT_OBJECT_0
					&& (::FindNextFile(hFind, &FindFileData)) );
		::FindClose(hFind);
	}


}


void CLeftView::onRightViewFolderSelected(CString strPath, UINT index)
{
	CExcerciseDoc *pDoc = this->GetDocument();

	CTreeCtrl &ctlFolders = this->GetTreeCtrl();

	HTREEITEM nodeSelected = ctlFolders.GetSelectedItem();

	ctlFolders.Expand(nodeSelected, TVE_EXPAND);

	int count =0;
	HTREEITEM hChild;
	hChild = ctlFolders.GetChildItem(nodeSelected);

 	if(index > 0){
		do{ 
			hChild = ctlFolders.GetNextItem(hChild, TVGN_NEXT );
			count++;
		}while(count < (int)index);
	}

	if(hChild != NULL){
		ctlFolders.SelectItem(hChild);
		ctlFolders.Expand(hChild, TVE_EXPAND);

		nodeSelected = hChild;

		CString strText = ctlFolders.GetItemText(nodeSelected);

		HTREEITEM hParent = nodeSelected;
		do{
			hParent = ctlFolders.GetParentItem(hParent);
			if(hParent != NULL)
				strText = ctlFolders.GetItemText(hParent) + "\\" + strText;

		}while(hParent !=NULL);

		CString strSelectedPath = strText +"*.*";
		pDoc->pRightView->displayFiles(strSelectedPath.GetBuffer(1));
	}
} 
