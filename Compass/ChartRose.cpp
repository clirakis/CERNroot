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
   fScale = 1.0/100.0;
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
void ChartRose::DrawLine(Float_t angle)
{
    const Float_t kScalex = 0.5;
    const Float_t kScaley = 0.5;

    point1->Rotate(angle);
    point2->Rotate(angle);

    if (fwh < fhh) 
    {                   // scale in oder to draw circle scale
	point1->Scale(kScalex, kScaley*fwh/fhh);
	point2->Scale(kScalex, kScaley*fwh/fhh);
    } 
    else 
    {
	point1->Scale(kScalex*fhh/fwh, kScaley);
	point2->Scale(kScalex*fhh/fwh, kScaley);
    }
    
    point1->Shift( kZero, kZero);              // move to center of pad
    point2->Shift( kZero, kZero);
    
    fPad->PaintLine( point1->GetX(), point1->GetY(),
		     point2->GetX(),point2->GetY());
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
    const Float_t Var = -4.0;   // temporary placeholder
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
    point1->SetXY(0.0, -kCrossDim*fScale/2.0);
    point2->SetXY(0.0,  kCrossDim*fScale/2.0);
    DrawLine( Variation);
    point1->SetXY(0.0, -kCrossDim*fScale/2.0);
    point2->SetXY(0.0,  kCrossDim*fScale/2.0);
    DrawLine( 90.0 + Variation);
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
	point1->SetXY(0.0, r1*fScale);
	point2->SetXY(0.0, r2*fScale);
	DrawLine(Angle);
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
    Float_t Angle  = 0.0;
    Float_t theta  = Variation;
    Float_t Radius = 0.0;
    char    text[128];
    TText   *tv;

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
	DrawLine(theta);

	tv = new TText();
	// Add text
	if (Outer)
	{
	    if (i%10==0)
	    {
		sprintf(text, "%3.0f", Angle);
		tv->SetText(point2->GetX(), point2->GetY(), text);
		tv->SetTextAlign(21);
		tv->SetTextSize(0.02);
		tv->SetTextColor(6);
		tv->SetTextAngle(-theta);
		tv->Draw();
	    }
	}
	else
	{
	    if (i%30==0)
	    {
		// Fill in after we get the above right. 
		sprintf(text, "%3.0f", Angle);
		tv->SetText(point2->GetX(), point2->GetY(), text);
		tv->SetTextAlign(21);
		tv->SetTextSize(0.02);
		tv->SetTextColor(6);
		tv->SetTextAngle(-Angle);
		tv->Draw();
	    }
	}
	Angle = Angle + 1.0;
	theta = Angle + Variation;
    }
}
