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
   fPad->SetLineWidth(1);
   fScale = 1.0/120.0;
   fMagnetic = Magnetic * TMath::DegToRad();
   SetBit(kCanDelete);
   fpoint1 = new RosePoints();
   fpoint2 = new RosePoints();
   fArrow  = new TArrow();
   fArrow->SetLineColor(6);
   fArrow->SetFillColor(6);
   fArrow->SetArrowSize(2.0*fScale);

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
    delete fpoint1;
    delete fpoint2;
    delete fArrow;
}
/**
 ******************************************************************
 *
 * Function Name : DrawLine
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

    fpoint1->Rotate(angle);
    fpoint2->Rotate(angle);

    if (fwh < fhh) 
    {                   // scale in oder to draw circle scale
	fpoint1->Scale(kScalex, kScaley*fwh/fhh);
	fpoint2->Scale(kScalex, kScaley*fwh/fhh);
    } 
    else 
    {
	fpoint1->Scale(kScalex*fhh/fwh, kScaley);
	fpoint2->Scale(kScalex*fhh/fwh, kScaley);
    }
    
    fpoint1->Shift( kZero, kZero);              // move to center of pad
    fpoint2->Shift( kZero, kZero);
    fPad->PaintLine( fpoint1->GetX(), fpoint1->GetY(),
		     fpoint2->GetX(), fpoint2->GetY());
}
/**
 ******************************************************************
 *
 * Function Name : MagneticNorth
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
void ChartRose::MagneticNorth(Float_t Angle)
{
    const Float_t kScalex = 0.5;
    const Float_t kScaley = 0.5;
    fpoint1->SetXY(0.0, kInnerMag[2]*fScale);
    fpoint2->SetXY(0.0, kInnerMag[3]*fScale);
    fpoint1->Rotate(Angle);
    fpoint2->Rotate(Angle);

    if (fwh < fhh) 
    {                   // scale in oder to draw circle scale
	fpoint1->Scale(kScalex, kScaley*fwh/fhh);
	fpoint2->Scale(kScalex, kScaley*fwh/fhh);
    } 
    else 
    {
	fpoint1->Scale(kScalex*fhh/fwh, kScaley);
	fpoint2->Scale(kScalex*fhh/fwh, kScaley);
    }    
    fpoint1->Shift( kZero, kZero);              // move to center of pad
    fpoint2->Shift( kZero, kZero);

    fArrow->SetX1( fpoint1->GetX());
    fArrow->SetY1( fpoint1->GetY());
    fArrow->SetX2( fpoint2->GetX());
    fArrow->SetY2( fpoint2->GetY());
    fArrow->Paint("|>");
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
    MagneticNorth(Var);
    Cardinal(Var);
    // True North major markings
    MajorPoints(kOuterMajorPoints, kOuterMajorPoints+kOuterPoints);
    MajorPoints(kInnerMag[0], kInnerMag[1], Var);
    MajorPoints(kInnerMag[2], kInnerMag[3], Var);

    Ring();
    Ring(kFALSE, Var);

    Letters(Var, 0.0);

    NorthStar();

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
void ChartRose::MakeCenterCross(Float_t Variation)
{
    const Float_t kCrossDim    =  5.0;   // These are just an arbitray scale.
    fpoint1->SetXY(0.0, -kCrossDim*fScale/2.0);
    fpoint2->SetXY(0.0,  kCrossDim*fScale/2.0);
    DrawLine( Variation);
    fpoint1->SetXY(0.0, -kCrossDim*fScale/2.0);
    fpoint2->SetXY(0.0,  kCrossDim*fScale/2.0);
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
    // 3 longish lines along true north. 
    Float_t Angle = Variation;

    for (UInt_t i=0; i<4; i++)
    {
	fpoint1->SetXY(0.0, r1*fScale);
	fpoint2->SetXY(0.0, r2*fScale);
	Angle = Angle + 90.0;
	DrawLine(Angle);
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
	fpoint1->SetXY(0.0, y0);
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
	fpoint2->SetXY(0.0, y1);
	DrawLine(theta);

	tv = new TText();
	// Add text
	if (Outer)
	{
	    if (i%10==0)
	    {
		sprintf(text, "%3.0f", Angle);
		tv->SetText(fpoint2->GetX(), fpoint2->GetY(), text);
		tv->SetTextAlign(21);
		tv->SetTextSize(0.02);
		tv->SetTextColor(6);
		if (Angle <= 90.0)
		{
		    tv->SetTextAngle(-theta);
		}
		else if (Angle < 270.0)
		{
		    tv->SetTextAngle(180.0 - theta);
		    tv->SetTextAlign(23);
		}
		else
		{
		    tv->SetTextAngle(-theta);
		}
		tv->Paint(); // Draw goes to paint
	    }
	}
	else
	{
	    if (i%30==0)
	    {
		// Fill in after we get the above right. 
		sprintf(text, "%3.0f", Angle);
		tv->SetText(fpoint2->GetX(), fpoint2->GetY(), text);
		tv->SetTextAlign(21);
		tv->SetTextSize(0.02);
		tv->SetTextColor(6);
		tv->SetTextAngle(-Angle);
		tv->Paint();
	    }
	}
	Angle = Angle + 1.0;
	theta = Angle + Variation;
    }
}
/**
 ******************************************************************
 *
 * Function Name : NorthStar
 *
 * Description : 10 triangles, 2 types. One filled the other not. 
 *
 * Angles:
 *      at base 10 looking up 18 degrees
 *      5 points looking up   75 degrees
 *
 * Lengths:
 *         Vertical: 5
 *         Side:     4
 *         
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
void ChartRose::NorthStar(void)
{
    const Float_t kRadius = 105.0;   // Center of Star
    const Float_t theta   = 36.0 * TMath::DegToRad();
    Float_t X[4], Y[4];

    X[0] = kZero;
    Y[0] = kRadius*fScale*kScaley + kZero;
    X[1] = kZero;
    Y[1] = (5.0 + kRadius)*fScale*kScaley + kZero;
    X[2] = (2.0*sin(theta))*fScale*kScaley + kZero;
    Y[2] = (2.0*cos(theta) + kRadius)*fScale*kScaley + kZero;
    X[3] = kZero;
    Y[3] = kRadius*fScale*kScaley + kZero;
    for (Int_t i=0;i<4;i++) cout << X[i] << " " << Y[i] << endl;
    TPolyLine *pl = new TPolyLine(4, X, Y);
    pl->SetFillColor(2);
    pl->Paint("f");

#if 0
    const Float_t kScalex = 0.5;
    const Float_t kScaley = 0.5;

    fpoint1->Rotate(angle);
    fpoint2->Rotate(angle);

    if (fwh < fhh) 
    {                   // scale in oder to draw circle scale
	fpoint1->Scale(kScalex, kScaley*fwh/fhh);
	fpoint2->Scale(kScalex, kScaley*fwh/fhh);
    } 
    else 
    {
	fpoint1->Scale(kScalex*fhh/fwh, kScaley);
	fpoint2->Scale(kScalex*fhh/fwh, kScaley);
    }
    
    fpoint1->Shift( kZero, kZero);              // move to center of pad
    fpoint2->Shift( kZero, kZero);
    fPad->PaintLine( fpoint1->GetX(), fpoint1->GetY(),
		     fpoint2->GetX(), fpoint2->GetY());
#endif
}
/**
 ******************************************************************
 *
 * Function Name : Cardinal
 *
 * Description : Make the inner most magnetic ring of the 
 *               cardinal points
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
void ChartRose::Cardinal(Float_t Variation)
{
    const Float_t Radius = 57.0;
    const Float_t kTic1  = 6.5;   // Modulo 4 units
    const Float_t kTic2  = 3.0;   // Modulo 2
    const Float_t kTic3  = 2.0;   // Modulo 1
    const Int_t   NTic   = 32*4;  // 32 major points
    const Float_t dTheta = 360.0/((Float_t)NTic);
    Float_t y0, y1;
	
    // Draw some other static features
    // 4 longish lines along true north. 
    Float_t Angle  = Variation;

    // 32 major tics total, offset by Variation
    for (UInt_t i=0; i<NTic; i++)
    {
	y0 = Radius*fScale;
	fpoint1->SetXY(0.0, y0);
	if (i%4==0)
	{
	    // 10 degree tics
	    y1 = y0 - kTic1*fScale;
	}
	else if (i%2==0)
	{
	    // 5 degree tics
	    y1 = y0 - kTic2*fScale;
	}
	else
	{
	    // 1 degree tics
	    y1 = y0 - kTic3*fScale;
	}
	fpoint2->SetXY(0.0, y1);
	DrawLine(Angle);

	Angle = Angle + dTheta;
    }
}
/**
 ******************************************************************
 *
 * Function Name : PlotLetter
 *
 * Description : Make the letters pertaining to the Variation
 *               
 * Inputs :
 *    Radius - to put them at. (Degrees)
 *    Variation - Angular variation (for rotating the text.) (Degrees)
 *    text - to put a letter at a time, each letter is slightly rotated. 
 *    At the top or bottom of the circle 
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
void ChartRose::PlotLetter(Float_t Radius, Float_t Variation, 
			   const char *text, Bool_t Top)
{
    const Float_t Ds     = 2.5 * 50.0; 
    const Float_t DAngle = Ds/Radius;    // 25.0/((Float_t)N);
    char  s[4];
    TText   *tv;
    Int_t   N      = strlen(text);
    Float_t Angle;

    if (Top)
    {
	Angle  = Variation - N/2*DAngle;
    }
    else
    {
	Angle  = 180.0 - Variation + N/2*DAngle;
    }

    // The word magnetic with the N 
    for (UInt_t i=0; i<N; i++)
    {
	fpoint1->SetXY(0.0, Radius*fScale);
	fpoint1->Rotate(Angle);
	if (fwh < fhh) 
	{                   // scale in oder to draw circle scale
	    fpoint1->Scale(kScalex, kScaley*fwh/fhh);
	} 
	else 
	{
	    fpoint1->Scale(kScalex*fhh/fwh, kScaley);
	}
    
	fpoint1->Shift( kZero, kZero);              // move to center of pad

	tv = new TText();
	// Add text
	sprintf(s, "%c", text[i]);
	tv->SetText(fpoint1->GetX(), fpoint1->GetY(), s);
	tv->SetTextSize(0.015);
	if (Top)
	{
	    tv->SetTextAlign(21);
	    tv->SetTextColor(6);
	    tv->SetTextAngle(-Angle);
	}
	else
	{
	    tv->SetTextAlign(23);
	    tv->SetTextColor(6);
	    tv->SetTextAngle(-Angle-180);
	}
	tv->Paint(); // Draw goes to paint

	if (Top)
	{
	    Angle = Angle + DAngle;
	}
	else
	{
	    Angle = Angle - DAngle;
	}
    }
}

/**
 ******************************************************************
 *
 * Function Name : Letters
 *
 * Description : Make the letters pertaining to the Variation
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
void ChartRose::Letters(Float_t Variation, Float_t AnnualIncrease)
{
    const Float_t Radius1 = 47.0;
    const Float_t Radius2 = 17.0;
    const char    *MText   = "MAGNETIC";
    char text[128];
    char Dir;
    Int_t Deg = floor(Variation);
    Int_t Min = floor((Variation - Deg)*60.0);
    if (Variation<0.0)
    {
	Dir = 'W';
    }
    else
    {
	Dir = 'E';
    }

    PlotLetter(Radius1, Variation, MText);
    //sprintf(text, "VAR %3d %2d %c (1985)", Deg, Min, Dir);
    sprintf(text, "VAR %d %d %c (1985)", Deg, Min, Dir);
    PlotLetter(Radius2, Variation, text);

    sprintf(text, "ANNUAL DECREASE 8");
    PlotLetter(Radius2, Variation, text, kFALSE);



}
