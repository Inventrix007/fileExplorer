
// ExcerciseDoc.h : interface of the CExcerciseDoc class
//


#pragma once

class CLeftView;
class CRightView;

class CExcerciseDoc : public CDocument
{
protected: // create from serialization only
	CExcerciseDoc();
	DECLARE_DYNCREATE(CExcerciseDoc)

// Attributes
public:

// Operations
public:
	CLeftView *pLeftView;
	CRightView *pRightView;

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CExcerciseDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
