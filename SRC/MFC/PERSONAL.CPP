/*
	 MiG Alley
	 Copyright (C) 1998, 1999, 2000, 2001 Empire Interactive (Europe) Ltd,
	 677 High Road, North Finchley, London N12 0DA

	 Please see the document licence.doc for the full licence agreement

2. LICENCE
 2.1 	
 	Subject to the provisions of this Agreement we now grant to you the 
 	following rights in respect of the Source Code:
  2.1.1 
  	the non-exclusive right to Exploit  the Source Code and Executable 
  	Code on any medium; and 
  2.1.2 
  	the non-exclusive right to create and distribute Derivative Works.
 2.2 	
 	Subject to the provisions of this Agreement we now grant you the
	following rights in respect of the Object Code:
  2.2.1 
	the non-exclusive right to Exploit the Object Code on the same
	terms and conditions set out in clause 3, provided that any
	distribution is done so on the terms of this Agreement and is
	accompanied by the Source Code and Executable Code (as
	applicable).

3. GENERAL OBLIGATIONS
 3.1 
 	In consideration of the licence granted in clause 2.1 you now agree:
  3.1.1 
	that when you distribute the Source Code or Executable Code or
	any Derivative Works to Recipients you will also include the
	terms of this Agreement;
  3.1.2 
	that when you make the Source Code, Executable Code or any
	Derivative Works ("Materials") available to download, you will
	ensure that Recipients must accept the terms of this Agreement
	before being allowed to download such Materials;
  3.1.3 
	that by Exploiting the Source Code or Executable Code you may
	not impose any further restrictions on a Recipient's subsequent
	Exploitation of the Source Code or Executable Code other than
	those contained in the terms and conditions of this Agreement;
  3.1.4 
	not (and not to allow any third party) to profit or make any
	charge for the Source Code, or Executable Code, any
	Exploitation of the Source Code or Executable Code, or for any
	Derivative Works;
  3.1.5 
	not to place any restrictions on the operability of the Source 
	Code;
  3.1.6 
	to attach prominent notices to any Derivative Works stating
	that you have changed the Source Code or Executable Code and to
	include the details anddate of such change; and
  3.1.7 
  	not to Exploit the Source Code or Executable Code otherwise than
	as expressly permitted by  this Agreement.

questions about this file may be asked at http://www.simhq.com/
*/

// personal.cpp : implementation file
//

#include "stdafx.h"
#include "mig.h"
#include "personal.h"
#include "rlistbox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#ifndef THIS_FILE_DEFINED
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CPilotsPersonalDetails dialog


CPilotsPersonalDetails::CPilotsPersonalDetails(int squadnum, int pilot, CWnd* pParent /*=NULL*/)
	: RowanDialog(CPilotsPersonalDetails::IDD, pParent)
{
		SetProjSpecific(-1,-1,squadnum, pilot);
		acnum =  24*groupnum + acnum;								  //RDH 02/06/99

	//{{AFX_DATA_INIT(CPilotsPersonalDetails)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPilotsPersonalDetails::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPilotsPersonalDetails)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX,IDC_NAME_PILOT1,m_IDC_NAME_PILOT1);
	DDX_Control(pDX,IDC_RLIST_DETAILS_PILOT1,m_IDC_RLIST_DETAILS_PILOT1);
	DDX_Control(pDX,IDC_NAME_PILOT2,m_IDC_NAME_PILOT2);
	DDX_Control(pDX,IDC_RLIST_DETAILS_PILOT2,m_IDC_RLIST_DETAILS_PILOT2);
	DDX_Control(pDX,IDC_RLIST_DETAILS_PILOT3,m_IDC_RLIST_DETAILS_PILOT3);
	DDX_Control(pDX,IDC_RLIST_DETAILS_PILOT4,m_IDC_RLIST_DETAILS_PILOT4);
	DDX_Control(pDX,IDC_NAME_PILOT3,m_IDC_NAME_PILOT3);
	DDX_Control(pDX,IDC_NAME_PILOT4,m_IDC_NAME_PILOT4);
		//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CPilotsPersonalDetails, CDialog)
	//{{AFX_MSG_MAP(CPilotsPersonalDetails)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPilotsPersonalDetails message handlers

bool CPilotsPersonalDetails::CommsPlayer(int pos, int& i)
{
	if (!RFullPanelDial::incomms)									  //JIM 19/05/99
		return false;												  //JIM 19/05/99
	i = 0;
//	while	(		(i < _DPlay.CurrPlayers)
	while	(		(i < MAXPLAYERS)
			)
	{
		if  (		(DPlay::H2H_Player[i].squadron == groupnum)
			&&		(DPlay::H2H_Player[i].position == pos)

			)
			return (true);
		i++;
	}
	return (false);
}


void	CPilotsPersonalDetails::DrawOnePilotDetail(CRStatic&	m_name, CRListBox& m_listbox, int i, int baseac)
{
		
	CRListBox* rlistbox;
	char buffer[20];
	CRStatic*   s;

		s=GETDLGITEM(name);
//DEADCODE JIM 21/05/99 		s->SetString(RESLIST(PILOTNAMES_START,MMC.Active_Pilots[24*groupnum+i+baseac].name.nameref));
			int player;
		rlistbox=GETDLGITEM(listbox);
		rlistbox->Clear();
		rlistbox->AddColumn(86);
		rlistbox->AddColumn(6);


		if (		(MMC.playersquadron == groupnum)
				&&	(MMC.playeracnum == (baseac + i - groupnum*24))
			)
			{
				s->SetForeColor(RGB(255,255,255));
				s->SetString(MMC.PlayerName);

				rlistbox->SetForeColor(RGB(255,255,255));
				rlistbox->AddString(RESSTRING(SORTIES),0);
				rlistbox->AddString(RESSTRING(COMBATS),0);
				rlistbox->AddString(RESSTRING(KILLS),0);
				rlistbox->AddString(RESSTRING(PILOT_LOSSES),0);

				int total[Debrief::LOSSES+1];
				for (Debrief::PlayerTypes i = Debrief::SORTIES; i <= Debrief::LOSSES; i++)
					total[i] = 0;

				for (i = Debrief::SORTIES; i <= Debrief::LOSSES; i++)
				{
					for (SquadSelect j = SQ_F86A; j < (SQ_B_NONFLYABLE); j++)
					{
						total[i] += MMC.debrief.playertotals[j][i];
					}
					//_itoa(total[i],buffer,10);
 					rlistbox->AddString(CSprintf("%i",total[i]),1);

				}




			}else if (CommsPlayer((baseac + i - groupnum*24), player))
			{
				s->SetForeColor(RGB(0,255,0));
				s->SetString(DPlay::H2H_Player[player].name);
			}else
			{
				s->SetForeColor(RGB(255,255,0));
				s->SetString(RESSTRING(PILOTNAMES_START + MMC.Active_Pilots[i+baseac].name.nameref));

				rlistbox->AddString(RESSTRING(SORTIES),0);
				_itoa(MMC.Active_Pilots[baseac+i].missionscompleted,buffer,10);
				rlistbox->AddString(buffer,1);

		//		rlistbox->AddString(RESSTRING(COMBATS),0);
		//		rlistbox->AddString("2",1);

				rlistbox->AddString(RESSTRING(KILLSLASTMSN),0);					  //RDH 26/02/99
				_itoa(MMC.Active_Pilots[baseac+i].airkillslastmsn,buffer,10);
				rlistbox->AddString(buffer,1);

				rlistbox->AddString(RESSTRING(KILLS),0);
				_itoa(MMC.Active_Pilots[baseac+i].kills,buffer,10);
				rlistbox->AddString(buffer,1);

		//		rlistbox->AddString(RESSTRING(AGGRESSION),0);
		//		rlistbox->AddString("High",1);

				rlistbox->AddString(RESSTRING(DAYSSINCEREST),0);
				_itoa(MMC.Active_Pilots[baseac+i].dayssincerest/3,buffer,10);
				rlistbox->AddString(buffer,1);


				rlistbox->AddString(RESSTRING(MORALE),0);
				int morale = MMC.Active_Pilots[baseac+i].morale/ MORALE_STAGE;
				rlistbox->AddString(RESLIST(MORALE_ROCKBOTTOM, morale),1);

				rlistbox->AddString(RESSTRING(FLYINGSKILL),0);
				int skill = MMC.Active_Pilots[baseac+i].FlyingSkill()/ SKILL_STAGE;
				rlistbox->AddString(RESLIST(SKILL_NOVICE, skill),1);

				rlistbox->AddString(RESSTRING(COMBATSKILL),0);
				skill = MMC.Active_Pilots[baseac+i].CombatSkill()/ SKILL_STAGE;
				rlistbox->AddString(RESLIST(SKILL_NOVICE, skill),1);

			}




}

BOOL CPilotsPersonalDetails::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int baseac = 4 * (acnum/4);

	DrawOnePilotDetail(m_IDC_NAME_PILOT1, m_IDC_RLIST_DETAILS_PILOT1, 0, baseac);

	DrawOnePilotDetail(m_IDC_NAME_PILOT2, m_IDC_RLIST_DETAILS_PILOT2, 1, baseac);

	DrawOnePilotDetail(m_IDC_NAME_PILOT3, m_IDC_RLIST_DETAILS_PILOT3, 2, baseac);

	DrawOnePilotDetail(m_IDC_NAME_PILOT4, m_IDC_RLIST_DETAILS_PILOT4, 3, baseac);




//DeadCode RDH 10Aug98 //	CRListBox* rlistbox;
//DeadCode RDH 10Aug98 	rlistbox=(CRListBox*)this->GetDlgItem(IDC_RLIST_DETAILS_PILOT2);
//DeadCode RDH 10Aug98 	rlistbox->Clear();
//DeadCode RDH 10Aug98 	rlistbox->AddColumn(87);
//DeadCode RDH 10Aug98 	rlistbox->AddColumn(60);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_SORTIES);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_COMBATS);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_KILLS);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_AGGRESION);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_MORALE);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_FLYINGSKILL);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_COMBATSKILL);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 
//DeadCode RDH 10Aug98 	rlistbox->AddString("2",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("2",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("1",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("High",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("Good",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("Good",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("Average",1);
//DeadCode RDH 10Aug98 
//DeadCode RDH 10Aug98 //	CRListBox* rlistbox;
//DeadCode RDH 10Aug98 	rlistbox=(CRListBox*)this->GetDlgItem(IDC_RLIST_DETAILS_PILOT3);
//DeadCode RDH 10Aug98 	rlistbox->Clear();
//DeadCode RDH 10Aug98 	rlistbox->AddColumn(87);
//DeadCode RDH 10Aug98 	rlistbox->AddColumn(60);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_SORTIES);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_COMBATS);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_KILLS);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_AGGRESION);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_MORALE);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_FLYINGSKILL);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_COMBATSKILL);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 
//DeadCode RDH 10Aug98 	rlistbox->AddString("2",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("2",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("1",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("High",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("Good",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("Good",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("Average",1);
//DeadCode RDH 10Aug98 
//DeadCode RDH 10Aug98 //	CRListBox* rlistbox;
//DeadCode RDH 10Aug98 	rlistbox=(CRListBox*)this->GetDlgItem(IDC_RLIST_DETAILS_PILOT4);
//DeadCode RDH 10Aug98 	rlistbox->Clear();
//DeadCode RDH 10Aug98 	rlistbox->AddColumn(87);
//DeadCode RDH 10Aug98 	rlistbox->AddColumn(60);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_SORTIES);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_COMBATS);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_KILLS);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_AGGRESION);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_MORALE);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_FLYINGSKILL);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 	string.LoadString(IDS_COMBATSKILL);
//DeadCode RDH 10Aug98 	rlistbox->AddString(string,0);
//DeadCode RDH 10Aug98 
//DeadCode RDH 10Aug98 	rlistbox->AddString("2",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("2",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("1",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("High",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("Good",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("Good",1);
//DeadCode RDH 10Aug98 	rlistbox->AddString("Average",1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
