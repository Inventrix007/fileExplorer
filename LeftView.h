
// LeftView.h : interface of the CLeftView class
//


#pragma once

class CExcerciseDoc;

class CLeftView : public CTreeView
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// Attributes
public:
	CExcerciseDoc* GetDocument();

// Operations
public:

// Overrides
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// create roots of left tree
	void createRoots(void);
private:
	HANDLE m_hStopEvent;
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	void Openfolder(void);
	void openFolder(CString CStrPath);
	void onRightViewFolderSelected(CString strPath, UINT index);
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CExcerciseDoc* CLeftView::GetDocument()
   { return reinterpret_cast<CExcerciseDoc*>(m_pDocument); }
#endif

