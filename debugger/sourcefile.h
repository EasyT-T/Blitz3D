#ifndef SOURCEFILE_H
#define SOURCEFILE_H

#include "stdafx.h"

class SourceFile : public CRichEditCtrl
{
public:
    SourceFile();

    ~SourceFile() override;

    void highLight(int row, int col);

    void setText(std::istream& in);

    DECLARE_DYNAMIC(SourceFile)

    DECLARE_MESSAGE_MAP()

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
    std::string is_line;
    std::istream* is_stream;
    int is_curs, is_linenum;

    void formatStreamLine();

    DWORD streamIn(LPBYTE buff, LONG cnt, LONG* done);
};

#endif
