cc = gcc 
link_f = -lGL -lGLU -lXaw3d -lXmu -lXt -lXext -lX11 -lm 
CC = g++

OBJECT = Graph.o graph.o getInput.o mapUtils.o \
action.o menu.o help.o util.o map.o codes.o smooth.o\
 opengl.o Opengl.o


.SUFFIXES:
.SUFFIXES: .C .def .h  .cc .c .o 
.c.o: ; $(cc)  -c $(flags_c) $*.c
.cc.o: ; $(CC) -c $(flags_cpp) $*.cc
BINARIES =  animator.bin fit sm agg mToI

all : $(BINARIES)

touch:
	rm -f menu.o
	
dep:
	makedepend $(flags_c) *.c *.cc util.gen

install:
	-cp $(BINARIES) miasma animator Description otto \
        state.map usa.map stateTable state.map.grid \
        ../bin

mToI : miasmaToIII.o readMiasma.o writeIII.o
	$(CC) $(debug_f) $(opt_f) miasmaToIII.o readMiasma.o \
writeIII.o util.o \
	 $(link_f) -o mToI 

fit : fiting.o anova.o readMiasma.o util.o 
	$(CC) $(debug_f) $(opt_f) fiting.o readMiasma.o anova.o util.o \
	 $(link_f) -o fit 

sm : smoo.o readMiasma.o util.o 
	$(CC) $(debug_f) $(opt_f) smoo.o readMiasma.o util.o \
	 $(link_f) -o sm 

agg : agg.o  readMiasma.o util.o 
	$(CC) $(debug_f) $(opt_f) agg.o  readMiasma.o util.o \
	 $(link_f) -o agg 

animator.bin :  $(OBJECT)
	$(CC) $(flags_cpp) $(OBJECT) $(link_f)  -o animator.bin

clean:
	rm -f *.o *~ \#*\# 



# DO NOT DELETE THIS LINE -- make depend depends on it.

action.o: /usr/include/stdio.h /usr/include/features.h
action.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
action.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
action.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
action.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
action.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
action.o: /usr/include/linux/stddef.h /usr/include/stdlib.h
action.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
action.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
action.o: /usr/include/sys/select.h /usr/include/bits/select.h
action.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
action.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
action.o: /usr/include/alloca.h /usr/include/math.h
action.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
action.o: /usr/include/bits/mathcalls.h /usr/include/X11/Xlib.h
action.o: /usr/include/X11/X.h /usr/include/X11/Xfuncproto.h
action.o: /usr/include/X11/Xosdefs.h /usr/include/X11/Intrinsic.h
action.o: /usr/include/X11/Xutil.h /usr/include/X11/keysym.h
action.o: /usr/include/X11/keysymdef.h /usr/include/X11/Xresource.h
action.o: /usr/include/string.h /usr/include/X11/Core.h
action.o: /usr/include/X11/Composite.h /usr/include/X11/Constraint.h
action.o: /usr/include/X11/Object.h /usr/include/X11/RectObj.h
action.o: /usr/include/X11/StringDefs.h /usr/include/X11/Shell.h
action.o: /usr/include/X11/SM/SMlib.h /usr/include/X11/SM/SM.h
action.o: /usr/include/X11/ICE/ICElib.h /usr/include/X11/ICE/ICE.h
action.o: /usr/include/X11/Xaw3d/Paned.h /usr/include/X11/Xmu/Converters.h
action.o: /usr/include/X11/Xaw3d/Scrollbar.h /usr/include/X11/Xaw3d/Box.h
action.o: /usr/include/X11/Xaw3d/Form.h /usr/include/X11/Xaw3d/Toggle.h
action.o: /usr/include/X11/Xaw3d/Command.h /usr/include/X11/Xaw3d/Label.h
action.o: /usr/include/X11/Xaw3d/Xaw3dP.h /usr/include/X11/IntrinsicP.h
action.o: /usr/include/X11/CoreP.h /usr/include/X11/CompositeP.h
action.o: /usr/include/X11/ConstrainP.h /usr/include/X11/ObjectP.h
action.o: /usr/include/X11/RectObjP.h /usr/include/X11/Xaw3d/Simple.h
action.o: /usr/include/X11/Xaw3d/List.h graph.h data.h action.h
getInput.o: /usr/include/X11/Xlib.h /usr/include/sys/types.h
getInput.o: /usr/include/features.h /usr/include/sys/cdefs.h
getInput.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
getInput.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
getInput.o: /usr/include/bits/typesizes.h /usr/include/time.h
getInput.o: /usr/include/endian.h /usr/include/bits/endian.h
getInput.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
getInput.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
getInput.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
getInput.o: /usr/include/bits/pthreadtypes.h /usr/include/X11/X.h
getInput.o: /usr/include/X11/Xfuncproto.h /usr/include/X11/Xosdefs.h
getInput.o: /usr/include/X11/Intrinsic.h /usr/include/X11/Xutil.h
getInput.o: /usr/include/X11/keysym.h /usr/include/X11/keysymdef.h
getInput.o: /usr/include/X11/Xresource.h /usr/include/string.h
getInput.o: /usr/include/X11/Core.h /usr/include/X11/Composite.h
getInput.o: /usr/include/X11/Constraint.h /usr/include/X11/Object.h
getInput.o: /usr/include/X11/RectObj.h /usr/include/X11/cursorfont.h
getInput.o: /usr/include/X11/Shell.h /usr/include/X11/SM/SMlib.h
getInput.o: /usr/include/X11/SM/SM.h /usr/include/X11/ICE/ICElib.h
getInput.o: /usr/include/X11/ICE/ICE.h /usr/include/X11/StringDefs.h
getInput.o: /usr/include/X11/Xaw3d/Toggle.h /usr/include/X11/Xaw3d/Command.h
getInput.o: /usr/include/X11/Xaw3d/Label.h /usr/include/X11/Xaw3d/Xaw3dP.h
getInput.o: /usr/include/X11/IntrinsicP.h /usr/include/X11/CoreP.h
getInput.o: /usr/include/X11/CompositeP.h /usr/include/X11/ConstrainP.h
getInput.o: /usr/include/X11/ObjectP.h /usr/include/X11/RectObjP.h
getInput.o: /usr/include/X11/Xaw3d/Simple.h /usr/include/X11/Xmu/Converters.h
getInput.o: /usr/include/X11/Xaw3d/Form.h /usr/include/X11/Xaw3d/AsciiText.h
getInput.o: /usr/include/X11/Xaw3d/Text.h /usr/include/X11/Xaw3d/TextSink.h
getInput.o: /usr/include/X11/Xaw3d/TextSrc.h
getInput.o: /usr/include/X11/Xaw3d/AsciiSrc.h
getInput.o: /usr/include/X11/Xaw3d/AsciiSink.h
getInput.o: /usr/include/X11/Xaw3d/MultiSrc.h /usr/include/stdio.h
getInput.o: /usr/include/libio.h /usr/include/_G_config.h
getInput.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
getInput.o: /usr/include/bits/sys_errlist.h graph.h data.h
graph.o: /usr/include/stdio.h /usr/include/features.h
graph.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
graph.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
graph.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
graph.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
graph.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
graph.o: /usr/include/linux/stddef.h /usr/include/stdlib.h
graph.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
graph.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
graph.o: /usr/include/sys/select.h /usr/include/bits/select.h
graph.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
graph.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
graph.o: /usr/include/alloca.h /usr/include/math.h
graph.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
graph.o: /usr/include/bits/mathcalls.h /usr/include/X11/Xlib.h
graph.o: /usr/include/X11/X.h /usr/include/X11/Xfuncproto.h
graph.o: /usr/include/X11/Xosdefs.h /usr/include/X11/Xutil.h
graph.o: /usr/include/X11/keysym.h /usr/include/X11/keysymdef.h
graph.o: /usr/include/X11/Xatom.h /usr/include/X11/cursorfont.h
graph.o: /usr/include/X11/Intrinsic.h /usr/include/X11/Xresource.h
graph.o: /usr/include/string.h /usr/include/X11/Core.h
graph.o: /usr/include/X11/Composite.h /usr/include/X11/Constraint.h
graph.o: /usr/include/X11/Object.h /usr/include/X11/RectObj.h
graph.o: /usr/include/X11/Shell.h /usr/include/X11/SM/SMlib.h
graph.o: /usr/include/X11/SM/SM.h /usr/include/X11/ICE/ICElib.h
graph.o: /usr/include/X11/ICE/ICE.h /usr/include/X11/StringDefs.h
graph.o: /usr/include/X11/Xaw3d/Paned.h /usr/include/X11/Xmu/Converters.h
graph.o: /usr/include/X11/Xaw3d/Label.h /usr/include/X11/Xaw3d/Xaw3dP.h
graph.o: /usr/include/X11/IntrinsicP.h /usr/include/X11/CoreP.h
graph.o: /usr/include/X11/CompositeP.h /usr/include/X11/ConstrainP.h
graph.o: /usr/include/X11/ObjectP.h /usr/include/X11/RectObjP.h
graph.o: /usr/include/X11/Xaw3d/Simple.h /usr/include/X11/Xaw3d/Box.h
graph.o: /usr/include/X11/Xaw3d/Scrollbar.h /usr/include/X11/Xaw3d/List.h
graph.o: /usr/include/X11/Xaw3d/Command.h /usr/include/X11/Xaw3d/MenuButton.h
graph.o: /usr/include/X11/Xaw3d/SimpleMenu.h /usr/include/X11/Xaw3d/SmeBSB.h
graph.o: /usr/include/X11/Xaw3d/Sme.h /usr/include/X11/Xaw3d/Viewport.h
graph.o: /usr/include/X11/Xaw3d/Form.h /usr/include/X11/Xaw3d/Reports.h
graph.o: graph.h data.h menu.h action.h /usr/include/signal.h
graph.o: /usr/include/bits/signum.h /usr/include/bits/siginfo.h
graph.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
graph.o: /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h
graph.o: /usr/include/sys/signal.h
help.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
help.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
help.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
help.o: /usr/include/bits/typesizes.h /usr/include/libio.h
help.o: /usr/include/_G_config.h /usr/include/wchar.h
help.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
help.o: /usr/include/X11/Xlib.h /usr/include/sys/types.h /usr/include/time.h
help.o: /usr/include/endian.h /usr/include/bits/endian.h
help.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
help.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
help.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
help.o: /usr/include/bits/pthreadtypes.h /usr/include/X11/X.h
help.o: /usr/include/X11/Xfuncproto.h /usr/include/X11/Xosdefs.h
help.o: /usr/include/X11/Xutil.h /usr/include/X11/keysym.h
help.o: /usr/include/X11/keysymdef.h /usr/include/X11/Xresource.h
help.o: /usr/include/X11/Intrinsic.h /usr/include/string.h
help.o: /usr/include/X11/Core.h /usr/include/X11/Composite.h
help.o: /usr/include/X11/Constraint.h /usr/include/X11/Object.h
help.o: /usr/include/X11/RectObj.h /usr/include/X11/IntrinsicP.h
help.o: /usr/include/X11/CoreP.h /usr/include/X11/CompositeP.h
help.o: /usr/include/X11/ConstrainP.h /usr/include/X11/ObjectP.h
help.o: /usr/include/X11/RectObjP.h /usr/include/X11/StringDefs.h
help.o: /usr/include/X11/Shell.h /usr/include/X11/SM/SMlib.h
help.o: /usr/include/X11/SM/SM.h /usr/include/X11/ICE/ICElib.h
help.o: /usr/include/X11/ICE/ICE.h /usr/include/X11/cursorfont.h
help.o: /usr/include/X11/bitmaps/target /usr/include/X11/Xaw3d/AsciiText.h
help.o: /usr/include/X11/Xaw3d/Xaw3dP.h /usr/include/X11/Xaw3d/Text.h
help.o: /usr/include/X11/Xaw3d/Simple.h /usr/include/X11/Xmu/Converters.h
help.o: /usr/include/X11/Xaw3d/TextSink.h /usr/include/X11/Xaw3d/TextSrc.h
help.o: /usr/include/X11/Xaw3d/AsciiSrc.h /usr/include/X11/Xaw3d/AsciiSink.h
help.o: /usr/include/X11/Xaw3d/MultiSrc.h /usr/include/X11/Xaw3d/AsciiTextP.h
help.o: /usr/include/X11/Xaw3d/TextP.h /usr/include/X11/Xaw3d/SimpleP.h
help.o: /usr/include/X11/Xaw3d/Box.h /usr/include/X11/Xaw3d/BoxP.h
help.o: /usr/include/X11/Xaw3d/Cardinals.h /usr/include/X11/Xaw3d/Command.h
help.o: /usr/include/X11/Xaw3d/Label.h /usr/include/X11/Xaw3d/CommandP.h
help.o: /usr/include/X11/Xaw3d/LabelP.h /usr/include/X11/Xaw3d/ThreeDP.h
help.o: /usr/include/X11/Xaw3d/ThreeD.h /usr/include/X11/Xaw3d/Dialog.h
help.o: /usr/include/X11/Xaw3d/Form.h /usr/include/X11/Xaw3d/DialogP.h
help.o: /usr/include/X11/Xaw3d/FormP.h /usr/include/X11/Xaw3d/MenuButton.h
help.o: /usr/include/X11/Xaw3d/Paned.h /usr/include/X11/Xaw3d/PanedP.h
help.o: /usr/include/X11/Xaw3d/Scrollbar.h
help.o: /usr/include/X11/Xaw3d/ScrollbarP.h
help.o: /usr/include/X11/Xaw3d/SimpleMenu.h /usr/include/X11/Xaw3d/Sme.h
help.o: /usr/include/X11/Xaw3d/SmeBSB.h /usr/include/X11/Xaw3d/Toggle.h
help.o: /usr/include/X11/Xaw3d/ToggleP.h graph.h data.h
lvr.o: /usr/include/sys/time.h /usr/include/features.h
lvr.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
lvr.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
lvr.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
lvr.o: /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/select.h
lvr.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
lvr.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
lvr.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
lvr.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
lvr.o: /usr/include/sys/types.h /usr/include/endian.h
lvr.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
lvr.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
lvr.o: /usr/include/sys/wait.h /usr/include/signal.h
lvr.o: /usr/include/bits/signum.h /usr/include/bits/siginfo.h
lvr.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
lvr.o: /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h
lvr.o: /usr/include/sys/resource.h /usr/include/bits/resource.h
lvr.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
lvr.o: /usr/include/sgtty.h /usr/include/sys/ioctl.h
lvr.o: /usr/include/bits/ioctls.h /usr/include/asm/ioctls.h
lvr.o: /usr/include/asm/ioctl.h /usr/include/asm-generic/ioctl.h
lvr.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
menu.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
menu.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
menu.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
menu.o: /usr/include/bits/typesizes.h /usr/include/libio.h
menu.o: /usr/include/_G_config.h /usr/include/wchar.h
menu.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
menu.o: /usr/include/linux/stddef.h /usr/include/stdlib.h
menu.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
menu.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
menu.o: /usr/include/sys/select.h /usr/include/bits/select.h
menu.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
menu.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
menu.o: /usr/include/alloca.h /usr/include/X11/Xlib.h /usr/include/X11/X.h
menu.o: /usr/include/X11/Xfuncproto.h /usr/include/X11/Xosdefs.h
menu.o: /usr/include/X11/StringDefs.h /usr/include/X11/Intrinsic.h
menu.o: /usr/include/X11/Xutil.h /usr/include/X11/keysym.h
menu.o: /usr/include/X11/keysymdef.h /usr/include/X11/Xresource.h
menu.o: /usr/include/string.h /usr/include/X11/Core.h
menu.o: /usr/include/X11/Composite.h /usr/include/X11/Constraint.h
menu.o: /usr/include/X11/Object.h /usr/include/X11/RectObj.h
menu.o: /usr/include/X11/Shell.h /usr/include/X11/SM/SMlib.h
menu.o: /usr/include/X11/SM/SM.h /usr/include/X11/ICE/ICElib.h
menu.o: /usr/include/X11/ICE/ICE.h /usr/include/X11/Xaw3d/Label.h
menu.o: /usr/include/X11/Xaw3d/Xaw3dP.h /usr/include/X11/IntrinsicP.h
menu.o: /usr/include/X11/CoreP.h /usr/include/X11/CompositeP.h
menu.o: /usr/include/X11/ConstrainP.h /usr/include/X11/ObjectP.h
menu.o: /usr/include/X11/RectObjP.h /usr/include/X11/Xaw3d/Simple.h
menu.o: /usr/include/X11/Xmu/Converters.h /usr/include/X11/Xaw3d/Form.h
menu.o: graph.h data.h
opengl.o: /usr/include/X11/Xlib.h /usr/include/sys/types.h
opengl.o: /usr/include/features.h /usr/include/sys/cdefs.h
opengl.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
opengl.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
opengl.o: /usr/include/bits/typesizes.h /usr/include/time.h
opengl.o: /usr/include/endian.h /usr/include/bits/endian.h
opengl.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
opengl.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
opengl.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
opengl.o: /usr/include/bits/pthreadtypes.h /usr/include/X11/X.h
opengl.o: /usr/include/X11/Xfuncproto.h /usr/include/X11/Xosdefs.h
opengl.o: /usr/include/GL/glx.h /usr/include/X11/Xutil.h
opengl.o: /usr/include/X11/keysym.h /usr/include/X11/keysymdef.h
opengl.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
opengl.o: /usr/include/inttypes.h /usr/include/stdint.h
opengl.o: /usr/include/bits/wchar.h /usr/include/GL/glxext.h graph.h data.h
opengl.o: /usr/include/X11/Intrinsic.h /usr/include/X11/Xresource.h
opengl.o: /usr/include/string.h /usr/include/X11/Core.h
opengl.o: /usr/include/X11/Composite.h /usr/include/X11/Constraint.h
opengl.o: /usr/include/X11/Object.h /usr/include/X11/RectObj.h
Graph.o: /usr/include/stdio.h /usr/include/features.h
Graph.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
Graph.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
Graph.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
Graph.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
Graph.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
Graph.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
Graph.o: /usr/include/endian.h /usr/include/bits/endian.h
Graph.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
Graph.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
Graph.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
Graph.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
Graph.o: /usr/include/math.h /usr/include/bits/huge_val.h
Graph.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
Graph.o: /usr/include/X11/Xlib.h /usr/include/X11/X.h
Graph.o: /usr/include/X11/Xfuncproto.h /usr/include/X11/Xosdefs.h
Graph.o: /usr/include/X11/StringDefs.h /usr/include/X11/cursorfont.h
Graph.o: /usr/include/X11/Intrinsic.h /usr/include/X11/Xutil.h
Graph.o: /usr/include/X11/keysym.h /usr/include/X11/keysymdef.h
Graph.o: /usr/include/X11/Xresource.h /usr/include/string.h
Graph.o: /usr/include/X11/Core.h /usr/include/X11/Composite.h
Graph.o: /usr/include/X11/Constraint.h /usr/include/X11/Object.h
Graph.o: /usr/include/X11/RectObj.h /usr/include/X11/Xaw3d/Simple.h
Graph.o: /usr/include/X11/Xaw3d/Xaw3dP.h /usr/include/X11/IntrinsicP.h
Graph.o: /usr/include/X11/CoreP.h /usr/include/X11/CompositeP.h
Graph.o: /usr/include/X11/ConstrainP.h /usr/include/X11/ObjectP.h
Graph.o: /usr/include/X11/RectObjP.h /usr/include/X11/Xmu/Converters.h
Graph.o: graph.h data.h map.h util.h util.gen.h smooth.h codes.h
Opengl.o: /usr/include/math.h /usr/include/features.h
Opengl.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
Opengl.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
Opengl.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
Opengl.o: /usr/include/bits/mathcalls.h /usr/include/X11/Xlib.h
Opengl.o: /usr/include/sys/types.h /usr/include/bits/types.h
Opengl.o: /usr/include/bits/typesizes.h /usr/include/time.h
Opengl.o: /usr/include/endian.h /usr/include/bits/endian.h
Opengl.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
Opengl.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
Opengl.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
Opengl.o: /usr/include/bits/pthreadtypes.h /usr/include/X11/X.h
Opengl.o: /usr/include/X11/Xfuncproto.h /usr/include/X11/Xosdefs.h
Opengl.o: /usr/include/GL/glx.h /usr/include/X11/Xutil.h
Opengl.o: /usr/include/X11/keysym.h /usr/include/X11/keysymdef.h
Opengl.o: /usr/include/GL/gl.h /usr/include/GL/glext.h
Opengl.o: /usr/include/inttypes.h /usr/include/stdint.h
Opengl.o: /usr/include/bits/wchar.h /usr/include/GL/glxext.h graph.h data.h
Opengl.o: /usr/include/X11/Intrinsic.h /usr/include/X11/Xresource.h
Opengl.o: /usr/include/string.h /usr/include/X11/Core.h
Opengl.o: /usr/include/X11/Composite.h /usr/include/X11/Constraint.h
Opengl.o: /usr/include/X11/Object.h /usr/include/X11/RectObj.h util.h
Opengl.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
Opengl.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
Opengl.o: /usr/include/bits/sys_errlist.h util.gen.h
agg.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
agg.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
agg.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
agg.o: /usr/include/bits/typesizes.h /usr/include/libio.h
agg.o: /usr/include/_G_config.h /usr/include/wchar.h
agg.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
agg.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
agg.o: /usr/include/endian.h /usr/include/bits/endian.h
agg.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
agg.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
agg.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
agg.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
agg.o: /usr/include/math.h /usr/include/bits/huge_val.h
agg.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
agg.o: /usr/include/malloc.h util.h util.gen.h data.h readMiasma.h
anova.o: /usr/include/stdlib.h /usr/include/features.h
anova.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
anova.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
anova.o: /usr/include/sys/types.h /usr/include/bits/types.h
anova.o: /usr/include/bits/typesizes.h /usr/include/time.h
anova.o: /usr/include/endian.h /usr/include/bits/endian.h
anova.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
anova.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
anova.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
anova.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
anova.o: /usr/include/math.h /usr/include/bits/huge_val.h
anova.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h anova.h
anova.o: util.h /usr/include/stdio.h /usr/include/libio.h
anova.o: /usr/include/_G_config.h /usr/include/wchar.h
anova.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
anova.o: util.gen.h
arr.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
arr.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
arr.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
arr.o: /usr/include/bits/typesizes.h /usr/include/libio.h
arr.o: /usr/include/_G_config.h /usr/include/wchar.h
arr.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h arr.h
array.o: /usr/include/stdio.h /usr/include/features.h
array.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
array.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
array.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
array.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
array.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
array.o: array.h
codes.o: /usr/include/stdio.h /usr/include/features.h
codes.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
codes.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
codes.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
codes.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
codes.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
codes.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
codes.o: /usr/include/endian.h /usr/include/bits/endian.h
codes.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
codes.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
codes.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
codes.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
codes.o: /usr/include/strings.h codes.h
fiting.o: /usr/include/stdio.h /usr/include/features.h
fiting.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
fiting.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
fiting.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
fiting.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
fiting.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
fiting.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
fiting.o: /usr/include/endian.h /usr/include/bits/endian.h
fiting.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
fiting.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
fiting.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
fiting.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
fiting.o: /usr/include/math.h /usr/include/bits/huge_val.h
fiting.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
fiting.o: /usr/include/malloc.h util.h util.gen.h data.h anova.h readMiasma.h
map.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
map.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
map.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
map.o: /usr/include/bits/typesizes.h /usr/include/libio.h
map.o: /usr/include/_G_config.h /usr/include/wchar.h
map.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
map.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
map.o: /usr/include/endian.h /usr/include/bits/endian.h
map.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
map.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
map.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
map.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
map.o: /usr/include/malloc.h map.h util.h util.gen.h graph.h data.h
map.o: /usr/include/X11/Xlib.h /usr/include/X11/X.h
map.o: /usr/include/X11/Xfuncproto.h /usr/include/X11/Xosdefs.h
map.o: /usr/include/X11/Intrinsic.h /usr/include/X11/Xutil.h
map.o: /usr/include/X11/keysym.h /usr/include/X11/keysymdef.h
map.o: /usr/include/X11/Xresource.h /usr/include/string.h
map.o: /usr/include/X11/Core.h /usr/include/X11/Composite.h
map.o: /usr/include/X11/Constraint.h /usr/include/X11/Object.h
map.o: /usr/include/X11/RectObj.h
mapUtils.o: /usr/include/stdlib.h /usr/include/features.h
mapUtils.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
mapUtils.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
mapUtils.o: /usr/include/sys/types.h /usr/include/bits/types.h
mapUtils.o: /usr/include/bits/typesizes.h /usr/include/time.h
mapUtils.o: /usr/include/endian.h /usr/include/bits/endian.h
mapUtils.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
mapUtils.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
mapUtils.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
mapUtils.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h lvr.h
mapUtils.o: map.h util.h /usr/include/stdio.h /usr/include/libio.h
mapUtils.o: /usr/include/_G_config.h /usr/include/wchar.h
mapUtils.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
mapUtils.o: util.gen.h graph.h data.h /usr/include/X11/Xlib.h
mapUtils.o: /usr/include/X11/X.h /usr/include/X11/Xfuncproto.h
mapUtils.o: /usr/include/X11/Xosdefs.h /usr/include/X11/Intrinsic.h
mapUtils.o: /usr/include/X11/Xutil.h /usr/include/X11/keysym.h
mapUtils.o: /usr/include/X11/keysymdef.h /usr/include/X11/Xresource.h
mapUtils.o: /usr/include/string.h /usr/include/X11/Core.h
mapUtils.o: /usr/include/X11/Composite.h /usr/include/X11/Constraint.h
mapUtils.o: /usr/include/X11/Object.h /usr/include/X11/RectObj.h codes.h
mapUtils.o: /usr/include/math.h /usr/include/bits/huge_val.h
mapUtils.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
miasmaToIII.o: /usr/include/stdio.h /usr/include/features.h
miasmaToIII.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
miasmaToIII.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
miasmaToIII.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
miasmaToIII.o: /usr/include/libio.h /usr/include/_G_config.h
miasmaToIII.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
miasmaToIII.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
miasmaToIII.o: /usr/include/sys/types.h /usr/include/time.h
miasmaToIII.o: /usr/include/endian.h /usr/include/bits/endian.h
miasmaToIII.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
miasmaToIII.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
miasmaToIII.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
miasmaToIII.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
miasmaToIII.o: /usr/include/math.h /usr/include/bits/huge_val.h
miasmaToIII.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
miasmaToIII.o: util.h util.gen.h data.h readMiasma.h writeIII.h
readMiasma.o: /usr/include/stdio.h /usr/include/features.h
readMiasma.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
readMiasma.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
readMiasma.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
readMiasma.o: /usr/include/libio.h /usr/include/_G_config.h
readMiasma.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
readMiasma.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
readMiasma.o: /usr/include/sys/types.h /usr/include/time.h
readMiasma.o: /usr/include/endian.h /usr/include/bits/endian.h
readMiasma.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
readMiasma.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
readMiasma.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
readMiasma.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
readMiasma.o: /usr/include/strings.h util.h util.gen.h data.h
smoo.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
smoo.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
smoo.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
smoo.o: /usr/include/bits/typesizes.h /usr/include/libio.h
smoo.o: /usr/include/_G_config.h /usr/include/wchar.h
smoo.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
smoo.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
smoo.o: /usr/include/endian.h /usr/include/bits/endian.h
smoo.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
smoo.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
smoo.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
smoo.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
smoo.o: /usr/include/math.h /usr/include/bits/huge_val.h
smoo.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
smoo.o: /usr/include/malloc.h util.h util.gen.h data.h anova.h readMiasma.h
smooth.o: /usr/include/stdio.h /usr/include/features.h
smooth.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
smooth.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
smooth.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
smooth.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
smooth.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
smooth.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
smooth.o: /usr/include/endian.h /usr/include/bits/endian.h
smooth.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
smooth.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
smooth.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
smooth.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h smooth.h
smooth.o: util.h util.gen.h
test.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
test.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
test.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
test.o: /usr/include/bits/typesizes.h /usr/include/libio.h
test.o: /usr/include/_G_config.h /usr/include/wchar.h
test.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h arr.h
util.o: /usr/include/errno.h /usr/include/features.h /usr/include/sys/cdefs.h
util.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
util.o: /usr/include/gnu/stubs-32.h /usr/include/bits/errno.h
util.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
util.o: /usr/include/asm-generic/errno.h
util.o: /usr/include/asm-generic/errno-base.h /usr/include/stdlib.h
util.o: /usr/include/sys/types.h /usr/include/bits/types.h
util.o: /usr/include/bits/typesizes.h /usr/include/time.h
util.o: /usr/include/endian.h /usr/include/bits/endian.h
util.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
util.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
util.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
util.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h util.h
util.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
util.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
util.o: /usr/include/bits/sys_errlist.h util.gen.h util.gen
writeIII.o: /usr/include/stdio.h /usr/include/features.h
writeIII.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
writeIII.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
writeIII.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
writeIII.o: /usr/include/libio.h /usr/include/_G_config.h
writeIII.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
writeIII.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
writeIII.o: /usr/include/sys/types.h /usr/include/time.h
writeIII.o: /usr/include/endian.h /usr/include/bits/endian.h
writeIII.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
writeIII.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
writeIII.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
writeIII.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
writeIII.o: /usr/include/math.h /usr/include/bits/huge_val.h
writeIII.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h util.h
writeIII.o: util.gen.h writeIII.h
