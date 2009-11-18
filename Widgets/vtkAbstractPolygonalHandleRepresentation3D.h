/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkAbstractPolygonalHandleRepresentation3D.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkAbstractPolygonalHandleRepresentation3D - represent a user defined handle geometry in 3D while maintaining a fixed orientation w.r.t the camera.
// .SECTION Description
// This class serves as the geometrical representation of a vtkHandleWidget. 
// The handle can be represented by an arbitrary polygonal data (vtkPolyData),
// set via SetHandle(vtkPolyData *). The actual position of the handle 
// will be initially assumed to be (0,0,0). You can specify an offset from
// this position if desired. This class differs from 
// vtkPolygonalHandleRepresentation3D in that the handle will always remain
// front facing, ie it maintains a fixed orientation with respect to the 
// camera. This is done by using vtkFollowers internally to render the actors.
// .SECTION See Also
// vtkPolygonalHandleRepresentation3D vtkHandleRepresentation vtkHandleWidget


#ifndef __vtkAbstractPolygonalHandleRepresentation3D_h
#define __vtkAbstractPolygonalHandleRepresentation3D_h

#include "vtkHandleRepresentation.h"

class vtkProperty;
class vtkPolyDataMapper;
class vtkCellPicker;
class vtkTransformPolyDataFilter;
class vtkMatrixToLinearTransform;
class vtkMatrix4x4;
class vtkPolyData;
class vtkAbstractTransform;
class vtkActor;
class vtkFollower;
class vtkVectorText;

class VTK_WIDGETS_EXPORT vtkAbstractPolygonalHandleRepresentation3D 
                           : public vtkHandleRepresentation
{
public:

  // Description:
  // Standard methods for instances of this class.
  vtkTypeRevisionMacro(vtkAbstractPolygonalHandleRepresentation3D,vtkHandleRepresentation);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the position of the point in world and display coordinates.
  virtual void SetWorldPosition(double p[3]);
  virtual void SetDisplayPosition(double p[3]);

  // Description:
  // Set/get the handle polydata.
  void SetHandle( vtkPolyData * );
  vtkPolyData * GetHandle();

  // Description:
  // Set/Get the handle properties when unselected and selected.
  void SetProperty(vtkProperty*);
  void SetSelectedProperty(vtkProperty*);
  vtkGetObjectMacro(Property,vtkProperty);
  vtkGetObjectMacro(SelectedProperty,vtkProperty);

  // Description:
  // Get the transform used to transform the generic handle polydata before
  // placing it in the render window
  virtual vtkAbstractTransform * GetTransform();

  // Description:
  // Methods to make this class properly act like a vtkWidgetRepresentation.
  virtual void BuildRepresentation();
  virtual void StartWidgetInteraction(double eventPos[2]);
  virtual void WidgetInteraction(double eventPos[2]);
  virtual int ComputeInteractionState(int X, int Y, int modify=0);

  // Description:
  // Methods to make this class behave as a vtkProp.
  virtual void ShallowCopy(vtkProp *prop);
  virtual void GetActors(vtkPropCollection *);
  virtual void ReleaseGraphicsResources(vtkWindow *);
  virtual int RenderOpaqueGeometry(vtkViewport *viewport);
  virtual int RenderTranslucentPolygonalGeometry(vtkViewport *viewport);
  virtual int HasTranslucentPolygonalGeometry();
  virtual double *GetBounds();
  
  // Description:
  // A label may be associated with the seed. The string can be set via
  // SetLabelText. The visibility of the label can be turned on / off.
  vtkSetMacro( LabelVisibility, int );
  vtkGetMacro( LabelVisibility, int );
  vtkBooleanMacro( LabelVisibility, int );
  virtual void SetLabelText( const char * label );
  
  // Description:
  // Scale text (font size along each dimension).
  virtual void SetLabelTextScale( double scale[3] );
  virtual double * GetLabelTextScale();
  
protected:
  vtkAbstractPolygonalHandleRepresentation3D();
  ~vtkAbstractPolygonalHandleRepresentation3D();

  vtkActor                   * Actor;
  vtkPolyDataMapper          * Mapper;
  vtkTransformPolyDataFilter * HandleTransformFilter;
  vtkMatrixToLinearTransform * HandleTransform;
  vtkMatrix4x4               * HandleTransformMatrix;
  vtkCellPicker              * HandlePicker;
  double                       LastPickPosition[3];
  double                       LastEventPosition[2];
  int                          ConstraintAxis;
  vtkProperty                * Property;
  vtkProperty                * SelectedProperty;
  int                          WaitingForMotion;
  int                          WaitCount;

  void Highlight(int highlight);

  // Methods to manipulate the cursor
  void Translate(double *p1, double *p2);
  void Scale(double *p1, double *p2, double eventPos[2]);
  void MoveFocus(double *p1, double *p2);
  
  void CreateDefaultProperties();

  // Given a motion vector defined by p1 --> p2 (p1 and p2 are in 
  // world coordinates), the new display position of the handle center is 
  // populated into requestedDisplayPos. This is again only a request for the
  // new display position. It is up to the point placer to deduce the 
  // appropriate world co-ordinates that this display position will map into.
  // The placer may even disallow such a movement.
  void MoveFocusRequest( 
      double *p1, double *p2, double requestedDisplayPos[3] );

  int DetermineConstraintAxis(int constraint, double *x, double *startPickPos);

  // Description:
  // Update the actor position. Different subclasses handle this differently.
  // For instance vtkPolygonalHandleRepresentation3D updates the handle 
  // transformation and sets this on the handle. 
  // vtkOrientedPolygonalHandleRepresentation3D, which uses a vtkFollower to
  // keep the handle geometry facinig the camera handles this differently. This
  // is an opportunity for subclasses to update the actor's position etc each
  // time the handle is rendered.
  virtual void UpdateHandle();

  // Description:
  // Opportunity to update the label position and text during each render.
  virtual void UpdateLabel();
  
  // Handle the label.
  int                LabelVisibility;
  vtkFollower       *LabelTextActor;
  vtkPolyDataMapper *LabelTextMapper;
  vtkVectorText     *LabelTextInput;  
  bool               LabelAnnotationTextScaleInitialized;

private:
  vtkAbstractPolygonalHandleRepresentation3D(const vtkAbstractPolygonalHandleRepresentation3D&);  //Not implemented
  void operator=(const vtkAbstractPolygonalHandleRepresentation3D&);  //Not implemented
};

#endif


