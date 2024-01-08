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
    const Float_t kDiv1        =  5.0;   // Divisions around True and mag 
    const Float_t kDiv2        =  3.5;
    const Float_t kDiv3        =  2.0;

    //static RosePoints *point1 = new RosePoints();
    //static RosePoints *point2 = new RosePoints();

    Float_t x0,y0, x1, y1;      // Working variables. 
    Float_t Angle, dAngle;
    fwh = (Float_t)fPad->XtoPixel(fPad->GetX2());
    fhh = (Float_t)fPad->YtoPixel(fPad->GetY1());
    fAspect = fwh/fhh;

    MakeCenterCross();
    TrueNorthMajorPoints();

    Angle = 0.0;
    // Draw outer rose scale
    for (UInt_t i=0; i<360; i++)
    {
	y0 = kRadius1*fScale;   // 0 degrees, North. 
	point1->SetXY(0.0, y0);
	if (i%10==0)
	{
	    y1 = y0 + kDiv1*fScale;
	}
	else if (i%5==0)
	{
	    y1 = y0 + kDiv2*fScale;
	}
	else
	{
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

	Angle = Angle + 1.0;
    }
   
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
void ChartRose::MakeCenterCross(void)
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
void ChartRose::TrueNorthMajorPoints(void)
{
    Float_t x0,y0, x1, y1;      // Working variables. 
    // Draw some other static features
    // 4 longish lines along true north. 
    Float_t Angle = 0.0;
    for (UInt_t i=0; i<4; i++)
    {
	y0 = kOuterMajorPoints * fScale;
	y1 = y0 + kOuterPoints * fScale;
	point1->SetXY(0.0, y0);
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

	Angle = Angle + 90.0;
    }
}
