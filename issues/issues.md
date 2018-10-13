## dictionary payload error

When running under root6.10.04, in centos7, the following error will accour when doing the display:

~~~~
GUIInforCenterDic dictionary payload:7:10: fatal error: cannot open file './src/GUIInforCenter.h': No such file or directory
#include "src/GUIInforCenter.h"
         ^
Error in <TInterpreter::AutoParse>: Error parsing payload code for class GUIInforCenter with content:

#line 1 "GUIInforCenterDic dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "src/GUIInforCenter.h"

#undef  _BACKWARD_BACKWARD_WARNING_H

Error in <TClass::LoadClassInfo>: no interpreter information for class GUIInforCenter is available even though it has a TClass initialization routine.
Error in <TQObject::CheckConnectArgs>: signal GUIInforCenter::GUICanvasTabDraw(GUICanvasDataStream*) does not exist
~~~~
