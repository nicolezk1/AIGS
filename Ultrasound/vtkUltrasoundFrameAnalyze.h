/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkUltrasoundFrameAnalyze.h,v $
  Language:  C++
  Date:      $Date: 2003/02/02 20:48:30 $
  Version:   $Revision: 1.1 $


Copyright (c) 1993-2001 Ken Martin, Will Schroeder, Bill Lorensen 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither name of Ken Martin, Will Schroeder, or Bill Lorensen nor the names
   of any contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

 * Modified source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
// .NAME vtkUltrasoundFrameAnalyze - get information about an ultrasound image
// .SECTION Description
// vtkUltrasoundFrameAnalyze extracts relevant information from a raw video
// ultrasound image, and modifies the Output and Origin of the image
// according to the x and y graticules present in the image.  It also
// provides a stencil that can be used to clip out only the pizza-slice
// of ultrasound data from the raw image.

#ifndef __vtkUltrasoundFrameAnalyze_h
#define __vtkUltrasoundFrameAnalyze_h

#include "vtkImageToImageFilter.h"
#include "vtkUltrasoundImageStencilSource.h"

class VTK_EXPORT vtkUltrasoundFrameAnalyze : public vtkImageToImageFilter
{
public:
  static vtkUltrasoundFrameAnalyze *New();
  vtkTypeMacro(vtkUltrasoundFrameAnalyze,vtkImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Update all the information to be extracted from the image.
  // This only performs "Execute" if an execute is required, i.e. 
  // if the image has changed since the last execute.
  void Analyze();

  // Description:
  // Get the origin and spacing for the input image accoding to
  // the graticules (the rulers) in the image.
  vtkGetVector3Macro(Spacing, float);
  vtkGetVector3Macro(Origin, float);

  // Description:
  // Get the clipping extent for the part of the image that lies
  // within the rectangle that is defined by the two rulers.
  vtkGetVector6Macro(ClipExtent, int);

  // Description:
  // Get the rectangle that contains the actual ultrasound data,
  // I.e. (xmin, xmax, ymin, ymax).
  // This isn't completely functional yet, instead it simply returns
  // the ClipExtent converted into millimetre units according to the
  // Spacing and Origin.
  vtkGetVector4Macro(ClipRectangle, float);

  // Description:
  // Get a boolean value to specify whether the ultrasound image
  // is flipped across one or more axes, according to the position
  // of the white dot in the image.
  vtkGetVector3Macro(Flip, int);

  // Description:
  // Get the color levels that correspond to black and white.
  vtkGetMacro(BlackLevel, float);
  vtkGetMacro(WhiteLevel, float);

  // Description:
  // Get the fan angles and the fan depth
  vtkGetVector2Macro(FanAngles, float);
  vtkGetVector2Macro(FanOrigin, float);
  vtkGetMacro(FanDepth, float);

  // Description:
  // Get a stencil for clipping out just the image portion of the
  // video frame.  Either vtkImageStencil or vtkImageBlend can be
  // used to apply the stencil.
  vtkImageStencilData *GetStencil() {
    return this->StencilSource->GetOutput(); };

protected:
  vtkUltrasoundFrameAnalyze();
  ~vtkUltrasoundFrameAnalyze();

  void ExecuteInformation(vtkImageData *inData, vtkImageData *outData);
  void ExecuteInformation() {
    this->vtkImageToImageFilter::ExecuteInformation(); };
  void ExecuteData(vtkDataObject *data);

  float Spacing[3];
  float Origin[3];
  int ClipExtent[6];
  float ClipRectangle[6];
  int Flip[3];
  float BlackLevel;
  float WhiteLevel;
  float FanAngles[2];
  float FanOrigin[2];
  float FanDepth;

  int ClipGuess[6];

  vtkUltrasoundImageStencilSource* StencilSource;

private:
  vtkUltrasoundFrameAnalyze(const vtkUltrasoundFrameAnalyze&);
  void operator=(const vtkUltrasoundFrameAnalyze&);
};

#endif