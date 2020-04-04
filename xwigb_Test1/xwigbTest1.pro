TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    atopkge.c \
    axesbox.c \
    colormap.c \
    docpkge.c \
    drawcurve.c \
    ealloc.c \
    errpkge.c \
    getpars.c \
    image.c \
    legendbox.c \
    lib/abel.c \
    lib/airy.c \
    lib/alloc.c \
    lib/antialias.c \
    lib/axb.c \
    lib/bigmatrix.c \
    lib/butterworth.c \
    lib/complex.c \
    lib/complexd.c \
    lib/complexf.c \
    lib/complexfd.c \
    lib/convolution.c \
    lib/cubicspline.c \
    lib/dblas.c \
    lib/dge.c \
    lib/dpfafft.c \
    lib/exit.c \
    lib/frannor.c \
    lib/franuni.c \
    lib/hankel.c \
    lib/hilbert.c \
    lib/holbergd1.c \
    lib/ibmflt_.c \
    lib/intcub.c \
    lib/intl2b.c \
    lib/intlin.c \
    lib/intlinc.c \
    lib/intsinc8.c \
    lib/inttable8.c \
    lib/mkdiff.c \
    lib/mkhdiff.c \
    lib/mksinc.c \
    lib/mnewt.c \
    lib/pfafft.c \
    lib/polar.c \
    lib/printerplot.c \
    lib/quest.c \
    lib/ressinc8.c \
    lib/rfwtva.c \
    lib/rfwtvaint.c \
    lib/sblas.c \
    lib/scaxis.c \
    lib/sge.c \
    lib/shfs8r.c \
    lib/sinc.c \
    lib/sort.c \
    lib/sqr.c \
    lib/stoep.c \
    lib/strstuff.c \
    lib/svd.c \
    lib/swapbyte.c \
    lib/tempname.c \
    lib/tridiagonal.c \
    lib/vandermonde.c \
    lib/waveforms.c \
    lib/xcor.c \
    lib/xindex.c \
    lib/yclip.c \
    lib/yxtoxy.c \
    lib/zasc.c \
    lib/zebc.c \
    rubberbox.c \
    subcalls.c \
    window.c \
    windows.c \
    xContour.c \
    xwigb.c

HEADERS += \
    cwp.h \
    cwpcmaps.h \
    garnish.h \
    par.h \
    picking.h \
    xplot.h \
    xwindow.h

QT += x11extras
LIBS += -lX11 -lXext

QMAKE_LFLAGS += -no-pie
