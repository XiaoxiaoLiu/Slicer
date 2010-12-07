/*==============================================================================

  Program: 3D Slicer

  Copyright (c) 2010 Kitware Inc.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// QT includes
#include <QColor>
#include <QDebug>

// qMRML includes
#include "qMRMLModelDisplayNodeWidget.h"
#include "ui_qMRMLModelDisplayNodeWidget.h"

// MRML includes
#include <vtkMRMLColorTableNode.h>
#include <vtkMRMLModelDisplayNode.h>
#include <vtkMRMLModelNode.h>

// VTK includes
#include <vtkSmartPointer.h>

//------------------------------------------------------------------------------
class qMRMLModelDisplayNodeWidgetPrivate: public Ui_qMRMLModelDisplayNodeWidget
{
  Q_DECLARE_PUBLIC(qMRMLModelDisplayNodeWidget);

protected:
  qMRMLModelDisplayNodeWidget* const q_ptr;

public:
  qMRMLModelDisplayNodeWidgetPrivate(qMRMLModelDisplayNodeWidget& object);
  void init();

  vtkSmartPointer<vtkMRMLModelDisplayNode> MRMLModelDisplayNode;
};

//------------------------------------------------------------------------------
qMRMLModelDisplayNodeWidgetPrivate::qMRMLModelDisplayNodeWidgetPrivate(qMRMLModelDisplayNodeWidget& object)
  : q_ptr(&object)
{
}

//------------------------------------------------------------------------------
void qMRMLModelDisplayNodeWidgetPrivate::init()
{
  Q_Q(qMRMLModelDisplayNodeWidget);
  this->setupUi(q);
  
  QObject::connect(this->ScalarsVisibilityCheckBox, SIGNAL(toggled(bool)),
                   q, SLOT(setScalarsVisibility(bool)));
  QObject::connect(this->ActiveScalarComboBox, SIGNAL(currentArrayChanged(const QString&)),
                   q, SLOT(setActiveScalarName(const QString&)));
  QObject::connect(this->ScalarsColorTableComboBox, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
                   q, SLOT(setScalarsColorTable(vtkMRMLNode*)));
  q->setEnabled(this->MRMLModelDisplayNode.GetPointer() != 0);
}

//------------------------------------------------------------------------------
qMRMLModelDisplayNodeWidget::qMRMLModelDisplayNodeWidget(QWidget *_parent)
  : QWidget(_parent)
  , d_ptr(new qMRMLModelDisplayNodeWidgetPrivate(*this))
{
  Q_D(qMRMLModelDisplayNodeWidget);
  d->init();
}

//------------------------------------------------------------------------------
qMRMLModelDisplayNodeWidget::~qMRMLModelDisplayNodeWidget()
{
}


//------------------------------------------------------------------------------
vtkMRMLModelDisplayNode* qMRMLModelDisplayNodeWidget::mrmlModelDisplayNode()const
{
  Q_D(const qMRMLModelDisplayNodeWidget);
  return d->MRMLModelDisplayNode;
}

//------------------------------------------------------------------------------
void qMRMLModelDisplayNodeWidget::setMRMLModelNode(vtkMRMLNode* node)
{
  vtkMRMLModelNode* modelNode = vtkMRMLModelNode::SafeDownCast(node);
  this->setMRMLModelDisplayNode(modelNode ? modelNode->GetDisplayNode() : 0);
}

//------------------------------------------------------------------------------
void qMRMLModelDisplayNodeWidget::setMRMLModelDisplayNode(vtkMRMLNode* node)
{
  this->setMRMLModelDisplayNode(vtkMRMLModelDisplayNode::SafeDownCast(node));
}

//------------------------------------------------------------------------------
void qMRMLModelDisplayNodeWidget::setMRMLModelDisplayNode(vtkMRMLModelDisplayNode* ModelDisplayNode)
{
  Q_D(qMRMLModelDisplayNodeWidget);
  qvtkReconnect(d->MRMLModelDisplayNode, ModelDisplayNode, vtkCommand::ModifiedEvent,
                this, SLOT(updateWidgetFromMRML()));
  d->MRMLModelDisplayNode = ModelDisplayNode;
  d->MRMLDisplayNodeWidget->setMRMLDisplayNode(ModelDisplayNode);
  this->updateWidgetFromMRML();
}

//------------------------------------------------------------------------------
void qMRMLModelDisplayNodeWidget::setScalarsVisibility(bool visible)
{
  Q_D(qMRMLModelDisplayNodeWidget);
  if (!d->MRMLModelDisplayNode.GetPointer())
    {
    return;
    }
  d->MRMLModelDisplayNode->SetScalarVisibility(visible);
}

//------------------------------------------------------------------------------
bool qMRMLModelDisplayNodeWidget::scalarsVisibility()const
{
  Q_D(const qMRMLModelDisplayNodeWidget);
  return d->ScalarsVisibilityCheckBox->isChecked();
}

//------------------------------------------------------------------------------
void qMRMLModelDisplayNodeWidget::setActiveScalarName(const QString& arrayName)
{
  Q_D(qMRMLModelDisplayNodeWidget);
  if (!d->MRMLModelDisplayNode.GetPointer())
    {
    return;
    }
  d->MRMLModelDisplayNode->SetActiveScalarName(arrayName.toLatin1());
}

//------------------------------------------------------------------------------
QString qMRMLModelDisplayNodeWidget::activeScalarName()const
{
  Q_D(const qMRMLModelDisplayNodeWidget);
  // TODO: use currentArrayName()
  vtkDataArray* dataArray = d->ActiveScalarComboBox->currentArray();
  return dataArray ? dataArray->GetName() : "";
}

//------------------------------------------------------------------------------
void qMRMLModelDisplayNodeWidget::setScalarsColorTable(vtkMRMLNode* colorNode)
{
  this->setScalarsColorTable(vtkMRMLColorTableNode::SafeDownCast(colorNode));
}

//------------------------------------------------------------------------------
void qMRMLModelDisplayNodeWidget::setScalarsColorTable(vtkMRMLColorTableNode* colorNode)
{
  Q_D(qMRMLModelDisplayNodeWidget);
  if (!d->MRMLModelDisplayNode.GetPointer())
    {
    return;
    }
  d->MRMLModelDisplayNode->SetAndObserveColorNodeID(colorNode ? colorNode->GetID() : "");
}

//------------------------------------------------------------------------------
vtkMRMLColorTableNode* qMRMLModelDisplayNodeWidget::scalarsColorTable()const
{
  Q_D(const qMRMLModelDisplayNodeWidget);
  return vtkMRMLColorTableNode::SafeDownCast(
    d->ScalarsColorTableComboBox->currentNode());
}

//------------------------------------------------------------------------------
void qMRMLModelDisplayNodeWidget::updateWidgetFromMRML()
{
  Q_D(qMRMLModelDisplayNodeWidget);
  this->setEnabled(d->MRMLModelDisplayNode.GetPointer() != 0);
  if (!d->MRMLModelDisplayNode.GetPointer())
    {
    return;
    }
  d->ScalarsVisibilityCheckBox->setChecked(d->MRMLModelDisplayNode->GetScalarVisibility());
  d->ActiveScalarComboBox->setDataSet(d->MRMLModelDisplayNode->GetPolyData());
  d->ActiveScalarComboBox->setCurrentArray(d->MRMLModelDisplayNode->GetActiveScalarName());
  d->ScalarsColorTableComboBox->setMRMLScene(d->MRMLModelDisplayNode->GetScene());
  d->ScalarsColorTableComboBox->setCurrentNode(d->MRMLModelDisplayNode->GetColorNodeID());
}