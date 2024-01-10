/**
 ******************************************************************
 *
 * Module Name : ChartRose.hh
 *
 * Author/Date : C.B. Lirakis / 06-Jan-24
 *
 * Description : Make an image of a modern day 
 *               compass rose fit for a chart. An amusement.
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 * a very old clock demo that Rene Brun did in root. I don't remember
 * where I found it, but I can not find it again. 
 *
 * http://root.cern.ch
 *
 *******************************************************************
 */
#ifndef __CHARTROSE_hh_
#define __CHARTROSE_hh_
#include <TTimer.h>
#include <TCanvas.h>
#include <TPolyLine.h>
#include <TDatime.h>
#include <TPoints.h>
#include <TMath.h>

// Outer compass rose
class RosePoints : public TPoints {

public:
   RosePoints(Coord_t x=0, Coord_t y=0) : TPoints(x,y) { }
   ~RosePoints() { }

   void SetXY(Coord_t x, Coord_t y) { SetX(x); SetY(y); }

   void Rotate(Float_t Angle)  // Rotates the coordinate system a angle degrees clockwise
   {

      Float_t rX, rY;
      Float_t theta = Angle*TMath::DegToRad();  

      rX = GetX()*TMath::Cos(theta)+GetY()*TMath::Sin(theta);
      rY = GetY()*TMath::Cos(theta)-GetX()*TMath::Sin(theta);
      SetXY(rX,rY);
   }

   void Scale(Float_t factorX, Float_t factorY) { SetX(GetX()*factorX); SetY(GetY()*factorY); }
   void Shift(Coord_t x, Coord_t y) { SetX(GetX()+x); SetY(GetY()+y); }
};


/// ChartRose documentation here. 
class ChartRose : public TObject {
public:
    /// Default Constructor 
    /// default magnetic offset to true north is 4.0 degrees.  
    ChartRose( Int_t csize=100, Float_t Magnetic=4.0);
    /// Default destructor
    virtual ~ChartRose();

    /*!
     * Description: 
     *   
     *
     * Arguments:
     *   
     *
     * Returns:
     *
     * Errors:
     *
     */
   void   Paint(Option_t *option);

   ClassDef(ChartRose,0)  // Chart based compass rose

private:
    TPad       *fPad;            // pad where this compass rose is drawn
    Float_t   fScale;
    Float_t   fMagnetic;        // Angular offset of magnetic north

    const Float_t kZero        =  0.5;
    const Float_t kRadius1     = 83.0;
    const Float_t kRadius2     = 60.0;
    const Float_t kOuterMajorPoints = 92.0;
    const Float_t kOuterPoints =  8.0;
    const Float_t kDiv1        =  5.0;   // Divisions around True and mag 
    const Float_t kDiv2        =  3.5;
    const Float_t kDiv3        =  2.0;
    const Float_t kInnerMag[4] =  {22.5,57.0, 69.0, 80.0};

    Float_t   fAspect;          // Aspect ratio

    // Break down the drawing a tad. Helper functions for each artifact. 
    void MakeCenterCross(Float_t Variation);
    void MajorPoints(Float_t r1, Float_t r2, Float_t Variation=0.0);
    // Standard compass ring
    void Ring(Bool_t Outer=kTRUE, Float_t Variation = 0.0); 

    RosePoints *point1;
    RosePoints *point2;
    Float_t    fwh, fhh;

};
#endif
