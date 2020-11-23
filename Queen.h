// Queen.h: interface for the CQueen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEEN_H__8C6C41B8_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_QUEEN_H__8C6C41B8_7899_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "EGG.H"

class CQueen : public CBee  {
  private:
	double m_CurrentSperm;
	double m_InitialSperm;
	double m_Strength;
	int m_Weggs;
	int m_Deggs;
	int m_Teggs; // Maintains total number of eggs laid on a day
	double m_DD, m_L, m_N, m_P, m_dd, m_l, m_n; // Variables needed to calculate 'E' 
	double m_MaxEggs;
	int m_CurQueenDay_1;  // The simulation day # on which the current queen started
	int m_EggLayingDelay;
	int CumWeggs;  // Test Only

	int m_Vars[5][2];  // Declares the array of MaxEggs and InitialSperm based on 1..5 queen strengths



  public:
	double GetPropDroneEggs();
	CQueen();
	CQueen(CQueen* oldQueen);
	void Serialize(CArchive &ar);
	void LayEggs(int LayDays, double DegreeDays, double DaylightHours, int NumForagers, 
		double LarvPerBee);
	CEgg* GetWeggs();
	CEgg* GetDeggs();
	int GetTeggs() const { return m_Teggs; }
	double GetDD() { return m_DD; }
	double GetL() { return m_L; }
	double GetN() { return m_N; }
	double GetP() { return m_P; }
	double Getdd() { return m_dd; }
	double Getl() { return m_l; }
	double Getn() { return m_n; }
	void SetInitialSperm(double sperm) {m_InitialSperm = sperm;}
	void SetCurrentSperm(double sperm) {m_CurrentSperm = sperm;}
	double GetInitialSperm() {return m_InitialSperm;}
	double GetCurrentSperm() {return m_CurrentSperm;}
	void SetStrength(double Strengh);
	double GetQueenStrength() {return m_Strength;}
	void SetMaxEggs(double max) {m_MaxEggs = max;}
	double GetMaxEggs() {return m_MaxEggs;}
	void SetEggLayingDelay(int Delay) {m_EggLayingDelay = Delay;}
	void SetDayOne(int DayNum) {m_CurQueenDay_1 = DayNum;}
	void ReQueen(int EggLayingDelay, double QueenStrength, int SimDayNum);
	virtual ~CQueen();

	CQueen& operator=(const CQueen& theQueen);

	// Return the value of L which is the DaylightHours based component
	// to the computation of laid eggs.
	double ComputeL(const double& DaylightHours) const;
};

#endif // !defined(AFX_QUEEN_H__8C6C41B8_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
