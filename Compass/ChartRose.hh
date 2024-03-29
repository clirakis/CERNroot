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
#include <TArrow.h>
#include <TMath.h>

// Outer compass rose
class RosePoints : public TPoints {

public:
   RosePoints(Coord_t x=0, Coord_t y=0) : TPoints(x,y) { }
   ~RosePoints() { }

    void SetXY(Coord_t x, Coord_t y) { SetX(x); SetY(y); };
    void SetXY(const RosePoints&p) { SetX(p.GetX()); SetY(p.GetY());};

   // Rotates the coordinate system a angle degrees clockwise
   void Rotate(Float_t Angle)  
   {

      Float_t rX, rY;
      Float_t theta = Angle*TMath::DegToRad();  

      rX = GetX()*TMath::Cos(theta)+GetY()*TMath::Sin(theta);
      rY = GetY()*TMath::Cos(theta)-GetX()*TMath::Sin(theta);
      SetXY(rX,rY);
   };

    void Scale(Float_t factorX, Float_t factorY) { SetX(GetX()*factorX); SetY(GetY()*factorY); };
    void Shift(Coord_t x, Coord_t y) { SetX(GetX()+x); SetY(GetY()+y); };
};


/// ChartRose documentation here. 
class ChartRose : public TObject {
public:
    /// Default Constructor 
    /// default magnetic offset to true north is 
    /// 41 18 30N  73 53 24W The declination is 12 45W 
    /// changing by 0 0.3E per year
    ChartRose( Int_t csize=600, Float_t Magnetic=-12.75);
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

    void SaveAs(const Char_t *Filename);

    ClassDef(ChartRose,0)  // Chart based compass rose

private:
    TPad      *fPad;            // pad where this compass rose is drawn
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
    const Float_t kArrow       = 12.0;
    const Float_t kScalex      = 0.5;
    const Float_t kScaley      = 0.5;

    Float_t       fAspect;          // Aspect ratio

    // Break down the drawing a tad. Helper functions for each artifact.
    void DrawLine(Float_t angle=0.0);
    void MakeCenterCross(Float_t Variation);
    void MajorPoints(Float_t r1, Float_t r2, Float_t Variation=0.0);
    void MagneticNorth(Float_t Angle);
    void NorthStar(void);
    void Cardinal(Float_t Variation);
    void PlotLetter(Float_t Radius,  Float_t Variation, const char *text, 
		    Bool_t Top = kTRUE);
    void Letters(Float_t Variation, Float_t AnnualIncrease);

    // Standard compass ring
    void Ring(Bool_t Outer=kTRUE, Float_t Variation = 0.0); 
    void PreparePoint(Float_t Angle);

    RosePoints *fpoint1;
    RosePoints *fpoint2;
    TArrow     *fArrow;
    Float_t    fwh, fhh;

};
#endif
