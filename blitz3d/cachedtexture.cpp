#include "cachedtexture.h"

int active_texs;

extern gxRuntime* gx_runtime;
extern gxGraphics* gx_graphics;

std::set<CachedTexture::Rep*> CachedTexture::rep_set;

static std::string path;

struct CachedTexture::Rep
{
    int ref_cnt;
    std::string file;
    int flags, w, h, first;
    std::vector<gxCanvas*> frames;

    Rep(const int w, const int h, const int flags, int cnt) :
        ref_cnt(1), flags(flags), w(w), h(h), first(0)
    {
        ++active_texs;
        while (cnt-- > 0)
        {
            if (gxCanvas* t = gx_graphics->createCanvas(w, h, flags))
            {
                frames.push_back(t);
            }
            else break;
        }
    }

    Rep(const std::string& f, const int flags, int w, int h, int first, int cnt) :
        ref_cnt(1), file(f), flags(flags), w(w), h(h), first(first)
    {
        ++active_texs;
        if (!(flags & gxCanvas::CANVAS_TEX_CUBE))
        {
            if (w <= 0 || h <= 0 || first < 0 || cnt <= 0)
            {
                w = h = first = 0;
                if (gxCanvas* t = gx_graphics->loadCanvas(f, flags))
                {
                    frames.push_back(t);
                }
                return;
            }
        }

        const int t_flags = flags & (
            gxCanvas::CANVAS_TEX_RGB |
            gxCanvas::CANVAS_TEX_ALPHA |
            gxCanvas::CANVAS_TEX_MASK |
            gxCanvas::CANVAS_TEX_HICOLOR) | gxCanvas::CANVAS_NONDISPLAY;

        gxCanvas* t = gx_graphics->loadCanvas(f, t_flags);
        if (!t) return;
        if (!t->getDepth())
        {
            gx_graphics->freeCanvas(t);
            return;
        }

        if (flags & gxCanvas::CANVAS_TEX_CUBE)
        {
            const int w = t->getWidth() / 6;
            if (w * 6 != t->getWidth()) return;
            const int h = t->getHeight();

            gxCanvas* tex = gx_graphics->createCanvas(w, h, flags);
            if (tex)
            {
                frames.push_back(tex);

                for (int face = 0; face < 6; ++face)
                {
                    tex->setCubeFace(face);
                    gx_graphics->copy(tex, 0, 0, tex->getWidth(), tex->getHeight(), t, face * w, 0, w, h);
                }
                tex->setCubeFace(1);
            }
        }
        else
        {
            const int x_tiles = t->getWidth() / w;
            const int y_tiles = t->getHeight() / h;
            if (first + cnt > x_tiles * y_tiles)
            {
                gx_graphics->freeCanvas(t);
                return;
            }
            int x = (first % x_tiles) * w;
            int y = (first / x_tiles) * h;
            while (cnt--)
            {
                gxCanvas* p = gx_graphics->createCanvas(w, h, flags);
                gx_graphics->copy(p, 0, 0, p->getWidth(), p->getHeight(), t, x, y, w, h);
                frames.push_back(p);
                x = x + w;
                if (x + w > t->getWidth())
                {
                    x = 0;
                    y = y + h;
                }
            }
        }
        gx_graphics->freeCanvas(t);
    }

    ~Rep()
    {
        --active_texs;
        for (int k = 0; k < frames.size(); ++k) gx_graphics->freeCanvas(frames[k]);
    }
};

CachedTexture::Rep* CachedTexture::findRep(const std::string& f, const int flags, const int w, const int h,
                                           const int first, const int cnt)
{
    for (std::set<Rep*>::const_iterator it = rep_set.begin(); it != rep_set.end(); ++it)
    {
        Rep* rep = *it;
        if (rep->file == f && rep->flags == flags && rep->w == w && rep->h == h && rep->first == first &&
            rep->frames.size() == cnt)
        {
            ++rep->ref_cnt;
            return rep;
        }
    }
    return nullptr;
}

CachedTexture::CachedTexture(const int w, const int h, const int flags, const int cnt) :
    rep(d_new Rep(w, h, flags, cnt))
{
}

CachedTexture::CachedTexture(const std::string& f_, const int flags, const int w, const int h, const int first,
                             const int cnt)
{
    std::string f = f_;
    if (f.substr(0, 2) == ".\\") f = f.substr(2);
    if (path.size())
    {
        const std::string t = path + tolower(filenamefile(f));
        if (rep = findRep(t, flags, w, h, first, cnt)) return;
        rep = d_new Rep(t, flags, w, h, first, cnt);
        if (rep->frames.size())
        {
            rep_set.insert(rep);
            return;
        }
        delete rep;
    }
    const std::string t = tolower(fullfilename(f));
    if (rep = findRep(t, flags, w, h, first, cnt)) return;
    rep = d_new Rep(t, flags, w, h, first, cnt);
    rep_set.insert(rep);
}

CachedTexture::CachedTexture(const CachedTexture& t) :
    rep(t.rep)
{
    ++rep->ref_cnt;
}

CachedTexture::~CachedTexture()
{
    if (!--rep->ref_cnt)
    {
        rep_set.erase(rep);
        delete rep;
    }
}

CachedTexture& CachedTexture::operator=(const CachedTexture& t)
{
    ++t.rep->ref_cnt;
    if (!--rep->ref_cnt)
    {
        rep_set.erase(rep);
        delete rep;
    }
    rep = t.rep;
    return *this;
}

std::string CachedTexture::getName() const
{
    return rep->file;
}

const std::vector<gxCanvas*>& CachedTexture::getFrames() const
{
    return rep->frames;
}

void CachedTexture::setPath(const std::string& t)
{
    path = tolower(t);
    if (const int sz = path.size())
    {
        if (path[sz - 1] != '/' && path[sz - 1] != '\\') path += '\\';
    }
}
