/********************************************************************
 *
 * Module Name : ChartRose.cpp
 *
 * Author/Date : C.B. Lirakis / 06-Jan-24
 *
 * Description : Generic ChartRose
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 * 7-Jan-24  Trying to add in text. 
 *
 * Classification : Unclassified
 *
 * References :
 *
 ********************************************************************/
// System includes.

#include <iostream>
using namespace std;
#include <string>
#include <cmath>

// Local Includes.
#include "ChartRose.hh"

/**
 ******************************************************************
 *
 * Function Name : ChartRose constructor
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
ChartRose::ChartRose (Int_t csize, Float_t Magnetic) : TObject()
{
   // Create a Compass rose in a new canvas.

   fPad = new TCanvas("Compass:canvas","Compass",-csize,csize);
   fPad->SetFillColor(0);     // white
   fPad->SetLineColor(6);     // pinkish. 
   fScale = 1.0/((Float_t) csize);
   fMagnetic = Magnetic * TMath::DegToRad();
   SetBit(kCanDelete);
   point1 = new RosePoints();
   point2 = new RosePoints();

   Draw();                    // append this Rose to fPad
}

/**
 ******************************************************************
 *
 * Function Name : ChartRose destructor
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
ChartRose::~ChartRose (void)
{
    delete point1;
    delete point2;
}


/**
 ******************************************************************
 *
 * Function Name : ChartRose function
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChartRose::Paint(Option_t *)
{
    const Float_t Var = -4.0;    // temporary placeholder
    //static RosePoints *point1 = new RosePoints();
    //static RosePoints *point2 = new RosePoints();

    Float_t x0,y0, x1, y1;      // Working variables. 
    Float_t Angle, dAngle;
    fwh = (Float_t)fPad->XtoPixel(fPad->GetX2());
    fhh = (Float_t)fPad->YtoPixel(fPad->GetY1());
    fAspect = fwh/fhh;

    MakeCenterCross(Var);
    // True North major markings
    MajorPoints(kOuterMajorPoints, kOuterMajorPoints+kOuterPoints);
    MajorPoints(kInnerMag[0], kInnerMag[1], Var);
    MajorPoints(kInnerMag[2], kInnerMag[3], Var);

    Ring();
    Ring(kFALSE, Var);

#if 0

   Float_t fwh = (Float_t)fPad->XtoPixel(fPad->GetX2());
   Float_t fhh = (Float_t)fPad->YtoPixel(fPad->GetY1());

   for (int i = 0; i < 60; i++ ) 
   {  
      // draw minute/hour ticks
      point1->SetXY(0.,0.9);

      if (!(i%5)) point2->SetXY(0.,0.8);       // hour  ticks  are longer
      else        point2->SetXY(0.,0.87);

      Float_t angle = 6.*i;
      point1->Rotate(angle);
      point2->Rotate(angle);

      if (wh < hh) {                   // scale in oder to draw circle scale
         point1->Scale(0.5,0.5*wh/hh);
         point2->Scale(0.5,0.5*wh/hh);
      } else {
         point1->Scale(0.5*hh/wh,0.5);
         point2->Scale(0.5*hh/wh,0.5);
      }

      point1->Shift(0.5,0.5);              // move to center of pad
      point2->Shift(0.5,0.5);

      fPad->PaintLine(point1->GetX(),point1->GetY(),point2->GetX(),point2->GetY());
   }
#endif
}
/**
 ******************************************************************
 *
 * Function Name : 
 *
 * Description : PUT IN VARIATION ANGLE. 
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChartRose::MakeCenterCross(Float_t Variation)
{
    const Float_t kCrossDim    =  5.0;   // These are just an arbitray scale.

    Float_t x0,y0, x1, y1;      // Working variables. 

    x0 = -kCrossDim*fScale/2.0 + kZero;
    x1 =  kCrossDim*fScale/2.0 + kZero;
    y0 = -kCrossDim*fScale/2.0*fAspect + kZero;
    y1 =  kCrossDim*fScale/2.0*fAspect + kZero;

    // Start simple make the cross in the middle. 
    fPad->PaintLine(x0, kZero, x1, kZero);
    // And rotate 90 degrees. 
    fPad->PaintLine(kZero, y0, kZero, y1);

}
/**
 ******************************************************************
 *
 * Function Name : 
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChartRose::MajorPoints(Float_t r1, Float_t r2, Float_t Variation)
{
    Float_t x0,y0, x1, y1;      // Working variables. 
    // Draw some other static features
    // 4 longish lines along true north. 
    Float_t Angle = Variation;
    for (UInt_t i=0; i<4; i++)
    {
//	y0 = kOuterMajorPoints * fScale;
//	y1 = y0 + kOuterPoints * fScale;
	point1->SetXY(0.0, r1*fScale);
	point2->SetXY(0.0, r2*fScale);

	point1->Rotate(Angle);
	point2->Rotate(Angle);

	if (fwh < fhh) 
	{                   // scale in oder to draw circle scale
	    point1->Scale(0.5,0.5*fwh/fhh);
	    point2->Scale(0.5,0.5*fwh/fhh);
	} 
	else 
	{
	    point1->Scale(0.5*fhh/fwh,0.5);
	    point2->Scale(0.5*fhh/fwh,0.5);
	}

	point1->Shift(0.5,0.5);              // move to center of pad
	point2->Shift(0.5,0.5);

	fPad->PaintLine(point1->GetX(),point1->GetY(),point2->GetX(),point2->GetY());

	Angle = Angle + 90.0;
    }
}
/**
 ******************************************************************
 *
 * Function Name : 
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void ChartRose::Ring(Bool_t Outer, Float_t Variation)
{
    Float_t x0,y0, x1, y1;      // Working variables. 
    // Draw some other static features
    // 4 longish lines along true north. 
    Float_t Angle  = Variation;
    Float_t Radius = 0.0;
    char    text[128];
    TText   *tv;

#if 0
    // https://root.cern.ch/doc/master/classTAttText.html
    TText *tv = new TText(0.5, 0.5, "0.0");
    tv->SetTextAlign(21);
    tv->SetTextSize(0.02);
    tv->SetTextColor(6);
    tv->Draw();
#endif

    if(Outer)
    {
	Radius = kRadius1;
    }
    else
    {
	Radius = kRadius2;
    }

    // Draw outer rose scale
    for (UInt_t i=0; i<360; i++)
    {
	y0 = Radius*fScale;   // 0 degrees, North. 
	point1->SetXY(0.0, y0);
	if (i%10==0)
	{
	    // 10 degree tics
	    y1 = y0 + kDiv1*fScale;
	}
	else if (i%5==0)
	{
	    // 5 degree tics
	    y1 = y0 + kDiv2*fScale;
	}
	else
	{
	    // 1 degree tics
	    y1 = y0 + kDiv3*fScale;
	}
	point2->SetXY(0.0, y1);

	point1->Rotate(Angle);
	point2->Rotate(Angle);

	if (fwh < fhh) 
	{                   // scale in oder to draw circle scale
	    point1->Scale(0.5,0.5*fwh/fhh);
	    point2->Scale(0.5,0.5*fwh/fhh);
	} 
	else 
	{
	    point1->Scale(0.5*fhh/fwh,0.5);
	    point2->Scale(0.5*fhh/fwh,0.5);
	}

	point1->Shift(0.5,0.5);              // move to center of pad
	point2->Shift(0.5,0.5);

	fPad->PaintLine(point1->GetX(),point1->GetY(),point2->GetX(),point2->GetY());
	tv = new TText();
	// Add text
	if (Outer)
	{
	    if (i%10==0)
	    {
		sprintf(text, "%3.0f", Angle);
		//tv = new TText(point2->GetX(), point2->GetY(), text);
		tv->SetText(point2->GetX(), point2->GetY(), text);
		tv->SetTextAlign(21);
		tv->SetTextSize(0.02);
		tv->SetTextColor(6);
		tv->SetTextAngle(-Angle);
		tv->Draw();
	    }
	}
	else
	{
	    if (i%30==0)
	    {
		// Fill in after we get the above right. 
	    }
	}
	Angle = Angle + 1.0;
    }
}
