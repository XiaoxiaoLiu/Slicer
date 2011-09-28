/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Danielle Pace, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qSlicerEMSegmentWorkflowWidgetStepPlugin_h
#define __qSlicerEMSegmentWorkflowWidgetStepPlugin_h

#include "qSlicerEMSegmentWorkflowWidgetsAbstractPlugin.h"

class Q_SLICER_QTMODULES_EMSEGMENT_WORKFLOW_WIDGETS_PLUGIN_EXPORT qSlicerEMSegmentWorkflowWidgetStepPlugin :
  public QObject,
  public qSlicerEMSegmentWorkflowWidgetsAbstractPlugin
{
  Q_OBJECT

public:
  qSlicerEMSegmentWorkflowWidgetStepPlugin(QObject *newParent = 0);
  
  QWidget *createWidget(QWidget *newParent);
  QString  domXml() const;
  QIcon    icon() const;
  QString  includeFile() const;
  bool     isContainer() const;
  QString  name() const;
  
};

#endif