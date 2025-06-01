#include "bbfilesystem.h"
#include <fstream>
#include "bbstream.h"
#include "std.h"

gxFileSystem *gx_filesys;

struct bbFile : public bbStream {
    std::filebuf *buf;

    bbFile(std::filebuf *f) : buf(f) {
    }

    ~bbFile() override
    {
        delete buf;
    }

    int read(char *buff, const int size) override
    {
        return buf->sgetn((char *) buff, size);
    }

    int write(const char *buff, const int size) override
    {
        return buf->sputn((char *) buff, size);
    }

    int avail() override
    {
        return buf->in_avail();
    }

    int eof() override
    {
        return buf->sgetc() == EOF;
    }
};

static std::set<bbFile *> file_set;

static inline void debugFile(bbFile *f) {
    if (debug) {
        if (!file_set.count(f))
            RTEX("File does not exist");
    }
}

static inline void debugDir(gxDir *d) {
    if (debug) {
        if (!gx_filesys->verifyDir(d))
            RTEX("Directory does not exist");
    }
}

static bbFile *open(BBStr *f, const int n) {
    const std::string t = *f;
    std::filebuf *buf = d_new std::filebuf();
    if (buf->open(t.c_str(), n | std::ios_base::binary)) {
        bbFile *f = d_new bbFile(buf);
        file_set.insert(f);
        return f;
    }
    delete buf;
    return nullptr;
}

bbFile *bbReadFile(BBStr *f) {
    return open(f, std::ios_base::in);
}

bbFile *bbWriteFile(BBStr *f) {
    return open(f, std::ios_base::out | std::ios_base::trunc);
}

bbFile *bbOpenFile(BBStr *f) {
    return open(f, std::ios_base::in | std::ios_base::out);
}

void bbCloseFile(bbFile *f) {
    debugFile(f);
    file_set.erase(f);
    delete f;
}

int bbFilePos(bbFile *f) {
    return f->buf->pubseekoff(0, std::ios_base::cur);
}

int bbSeekFile(bbFile *f, const int pos) {
    return f->buf->pubseekoff(pos, std::ios_base::beg);
}

gxDir *bbReadDir(BBStr *d) {
    const std::string t = *d;
    delete d;
    return gx_filesys->openDir(t, 0);
}

void bbCloseDir(gxDir *d) {
    gx_filesys->closeDir(d);
}

BBStr *bbNextFile(gxDir *d) {
    debugDir(d);
    return d_new BBStr(d->getNextFile());
}

BBStr *bbCurrentDir() {
    return d_new BBStr(gx_filesys->getCurrentDir());
}

void bbChangeDir(BBStr *d) {
    gx_filesys->setCurrentDir(*d);
    delete d;
}

void bbCreateDir(BBStr *d) {
    gx_filesys->createDir(*d);
    delete d;
}

void bbDeleteDir(BBStr *d) {
    gx_filesys->deleteDir(*d);
    delete d;
}

int bbFileType(BBStr *f) {
    const std::string t = *f;
    delete f;
    const int n = gx_filesys->getFileType(t);
    return n == gxFileSystem::FILE_TYPE_FILE ? 1 : (n == gxFileSystem::FILE_TYPE_DIR ? 2 : 0);
}

int bbFileSize(BBStr *f) {
    const std::string t = *f;
    delete f;
    return gx_filesys->getFileSize(t);
}

void bbCopyFile(BBStr *f, BBStr *to) {
    std::string src = *f, dest = *to;
    delete f;
    delete to;
    gx_filesys->copyFile(src, dest);
}

void bbDeleteFile(BBStr *f) {
    gx_filesys->deleteFile(*f);
    delete f;
}

bool filesystem_create() {
    if (gx_filesys = gx_runtime->openFileSystem(0)) {
        return true;
    }
    return false;
}

bool filesystem_destroy() {
    while (file_set.size()) bbCloseFile(*file_set.begin());
    gx_runtime->closeFileSystem(gx_filesys);
    return true;
}

void filesystem_link(void (*rtSym)(const char *, void *)) {
    rtSym("%OpenFile$filename", bbOpenFile);
    rtSym("%ReadFile$filename", bbReadFile);
    rtSym("%WriteFile$filename", bbWriteFile);
    rtSym("CloseFile%file_stream", bbCloseFile);
    rtSym("%FilePos%file_stream", bbFilePos);
    rtSym("%SeekFile%file_stream%pos", bbSeekFile);

    rtSym("%ReadDir$dirname", bbReadDir);
    rtSym("CloseDir%dir", bbCloseDir);
    rtSym("$NextFile%dir", bbNextFile);
    rtSym("$CurrentDir", bbCurrentDir);
    rtSym("ChangeDir$dir", bbChangeDir);
    rtSym("CreateDir$dir", bbCreateDir);
    rtSym("DeleteDir$dir", bbDeleteDir);

    rtSym("%FileSize$file", bbFileSize);
    rtSym("%FileType$file", bbFileType);
    rtSym("CopyFile$file$to", bbCopyFile);
    rtSym("DeleteFile$file", bbDeleteFile);
}
