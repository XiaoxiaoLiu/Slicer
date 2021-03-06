""" This module is a place holder for convenient functions allowing to interact with CLI."""

def createNode(module, parameters = None):
  '''Creates a new vtkMRMLCommandLineModuleNode for a specific module, with
  optional parameters'''
  import slicer.util
  widget = slicer.util.getModuleGui(module)
  if not widget:
    return None
  selector = widget.findChild("qMRMLNodeComboBox", "MRMLCommandLineModuleNodeSelector")
  if not selector:
    print "Could not find node selector for name '%s" % moduleName
    return None
  node = selector.addNode()
  setNodeParameters(node, parameters)
  return node

def setNodeParameters(node, parameters):
  '''Sets parameters for a vtkMRMLCommandLineModuleNode given a dictionary
  of (parameterName, parameterValue) pairs
  For vectors: provide a list, tuple or comma-separated string
  For enumerations, provide the single enumeration value
  For files and directories, provide a string
  For images, geometry, points and regions, provide a vtkMRMLNode
  '''
  import slicer
  if not node:
    return None
  if not parameters:
    return None
  for key, value in parameters.iteritems():
    if isinstance(value, basestring):
      node.SetParameterAsString(key, value)
    elif isinstance(value, bool):
      node.SetParameterAsBool(key, value)
    elif isinstance(value, int):
      node.SetParameterAsInt(key, value)
    elif isinstance(value, float):
      node.SetParameterAsDouble(key, value)
    elif isinstance(value, slicer.vtkMRMLNode):
      node.SetParameterAsString(key, value.GetID())
    elif isinstance(value, list) or isinstance(value, tuple):
      commaSeparatedString = str(value)
      commaSeparatedString = commaSeparatedString[1:len(commaSeparatedString)-1]
      node.SetParameterAsString(key, commaSeparatedString)
    #TODO: file support
    else:
      print "parameter ", key, " has unsupported type ", value.__class__.__name__

def run(module, node = None, parameters = None, wait_for_completion = False):
  '''Runs a CLI, optionally given a node with optional parameters, returning
  back the node (or the new one if created)'''
  import slicer.util
  if node:
    setNodeParameters(node, parameters)
  else:
    node = createNode(module, parameters)
    if not node:
      return

  logic = module.logic()

  if wait_for_completion:
      logic.ApplyAndWait(node)
  else:
      logic.Apply(node)
  #widget = slicer.util.getModuleGui(module)
  #if not widget:
  #  print "Could not find widget representation for module"
  #  return None
  #widget.setCurrentCommandLineModuleNode(node)
  #widget.apply()
  return node
  
def cancel(node):
  print "Not yet implemented"
