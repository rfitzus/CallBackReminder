// **************************************************
// Custom code for UD02Form
// Created: 22/06/2018 10:40:20 AM
// **************************************************
using System;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Windows.Forms;
using Ice.BO;
using Ice.UI;
using Ice.Lib;
using Ice.Adapters;
using Ice.Lib.Customization;
using Ice.Lib.ExtendedProps;
using Ice.Lib.Framework;
using Ice.Lib.Searches;
using Ice.UI.FormFunctions;
using System.Reflection;

//ADDED THESE
using Ice.Core;
using Erp.UI.App.CRMCallEntry;
using System.Xml;
using Erp.Adapters;
using Erp.BO;
using System.Collections;
using System.Diagnostics;

//CUSTOM ASSEMBLIES - Tools > Assembly Reference Manager > Add Custom Reference
//Ice.Core.Session
//ERP.UI.CRMCallEntry
//ERP.Adapters.CRMCall


public class Script
{
	// ** Wizard Insert Location - Do Not Remove 'Begin/End Wizard Added Module Level Variables' Comments! **
	// Begin Wizard Added Module Level Variables **

	Infragistics.Win.UltraWinDock.UltraDockManager dock;

	private EpiDataView edvUD02;
	// End Wizard Added Module Level Variables **

	// Add Custom Module Level Variables Here **

	public void InitializeCustomCode()
	{
		// ** Wizard Insert Location - Do not delete 'Begin/End Wizard Added Variable Initialization' lines **
		// Begin Wizard Added Variable Initialization

		Object obj = typeof(Ice.UI.App.UD02Entry.UD02Form).InvokeMember("baseDockManager", BindingFlags.Instance | BindingFlags.GetField | BindingFlags.NonPublic, null, UD02Form, null);
		dock =  (Infragistics.Win.UltraWinDock.UltraDockManager)obj;                  
		dock.DockAreas[0].Panes[0].Closed = true;

		this.edvUD02 = ((EpiDataView)(this.oTrans.EpiDataViews["UD02"]));
		this.edvUD02.EpiViewNotification += new EpiViewNotification(this.edvUD02_EpiViewNotification);
		// End Wizard Added Variable Initialization

		// Begin Wizard Added Custom Method Calls

		this.OneWeek_Btn.Click += new System.EventHandler(this.OneWeek_Btn_Click);
		this.NewCRMCall_Btn.Click += new System.EventHandler(this.NewCRMCall_Btn_Click);
		this.OneMonth_Btn.Click += new System.EventHandler(this.OneMonth_Btn_Click);
		// End Wizard Added Custom Method Calls
	}

	public void DestroyCustomCode()
	{
		// ** Wizard Insert Location - Do not delete 'Begin/End Wizard Added Object Disposal' lines **
		// Begin Wizard Added Object Disposal

		this.edvUD02.EpiViewNotification -= new EpiViewNotification(this.edvUD02_EpiViewNotification);
		this.edvUD02 = null;
		this.OneWeek_Btn.Click -= new System.EventHandler(this.OneWeek_Btn_Click);
		this.NewCRMCall_Btn.Click -= new System.EventHandler(this.NewCRMCall_Btn_Click);
		this.OneMonth_Btn.Click -= new System.EventHandler(this.OneMonth_Btn_Click);
		// End Wizard Added Object Disposal

		// Begin Custom Code Disposal

		// End Custom Code Disposal
	}

	private void edvUD02_EpiViewNotification(EpiDataView view, EpiNotifyArgs args)
	{
		// ** Argument Properties and Uses **
		// view.dataView[args.Row]["FieldName"]
		// args.Row, args.Column, args.Sender, args.NotifyType
		// NotifyType.Initialize, NotifyType.AddRow, NotifyType.DeleteRow, NotifyType.InitLastView, NotifyType.InitAndResetTreeNodes
		if ((args.NotifyType == EpiTransaction.NotifyType.AddRow))
		{
			if ((args.Row > -1))
			{
				DateTime currentDay = DateTime.Today;
	
				view.dataView[view.Row].BeginEdit();
				view.dataView[view.Row]["Key1"] = currentDay.ToString("d");
				view.dataView[view.Row]["Key2"] = (((Session)oTrans.Session).UserID).ToString();

				if (UD02Form.LaunchFormOptions != null) 
		    	{
					if (UD02Form.LaunchFormOptions.ContextValue != null) 
		    		{
		
						string Lfo = UD02Form.LaunchFormOptions.ContextValue.ToString();
		
						string[] values = Lfo.Split('|');  //Split String out between CustID and ShipToNum				
			
						view.dataView[view.Row]["Key3"] = values[3];
						view.dataView[view.Row]["Key4"] = values[4];
						view.dataView[view.Row]["Key5"] = values[5];
		
					}
		    	}

				view.dataView[view.Row].EndEdit();
			}
		}
	}

	private void UD02Form_Load(object sender, EventArgs args)
	{
		// Add Event Handler Code

		

				if (UD02Form.LaunchFormOptions != null) 
		    	{
					if (UD02Form.LaunchFormOptions.ContextValue != null) 
		    		{
		
						string Lfo = UD02Form.LaunchFormOptions.ContextValue.ToString();
		
						string[] values = Lfo.Split('|');  //Split String out between CustID and ShipToNum

						if(values[0] == "Open")
						{
							this.oTrans.GetByID(values[1],values[2],values[3],values[4],values[5]);
						}
						else
						{
							this.oTrans.GetNew();
						}
					}
		    	}

		
	}

	private void OneWeek_Btn_Click(object sender, System.EventArgs args)
	{
		// ** Place Event Handling Code Here **
		EpiDataView edv = (EpiDataView)oTrans.EpiDataViews["UD02"];

		DateTime currentDay = DateTime.Today; 
		edv.dataView[edv.Row].BeginEdit();
        edv.dataView[edv.Row]["Date01"] = currentDay.AddDays(1*7);
		edv.dataView[edv.Row].EndEdit();
		
	}

	private void NewCRMCall_Btn_Click(object sender, System.EventArgs args)
	{
		// ** Place Event Handling Code Here **

		string launchType = "AddNew";		

		string custID = string.Empty;
	    string key1 = string.Empty;
	    string key2 = string.Empty;
	    string key3 = string.Empty;
	    string relToFile = string.Empty;
	    int callSeqNum = 0;
	    int custNum = 0;
		string shipToNum = string.Empty;
	    bool LaunchOK = true;

		EpiDataView edv = (EpiDataView)oTrans.EpiDataViews["UD02"];
		
		try
		{
			if(edv.Row > -1)
			{
				custNum = Convert.ToInt32(edv.dataView[edv.Row]["Key3"].ToString());				
				custID = custNum.ToString();
				//shipToNum = CustomerList.dataView[CustomerList.Row]["ShipTo_ShipToNum"].ToString();
				relToFile = "customer";
				key1 = custNum.ToString();
				key2 = custNum.ToString();
			}
			else
			{
				LaunchOK = false;
			}
			
		}
		catch
		{
			LaunchOK = false;
		}
		if (LaunchOK)
		{
			XmlDocument xDoc = new XmlDocument();
			XmlNode xDocElem = xDoc.CreateElement("Test");
			XmlNode xNodeMode = xDoc.CreateElement("Mode");
			xNodeMode.InnerText = launchType;
			xDocElem.AppendChild(xNodeMode);
			xDoc.AppendChild(xDocElem);	
			CRMCallArgs crmArgs = new CRMCallArgs(relToFile, key1, key2, key3, callSeqNum.ToString(), custID);
			crmArgs.ValueIn = xDoc;
			ProcessCaller.LaunchForm(this.oTrans , "CRGO6100", crmArgs);
		}
	}

	private void OneMonth_Btn_Click(object sender, System.EventArgs args)
	{
		// ** Place Event Handling Code Here **

				EpiDataView edv = (EpiDataView)oTrans.EpiDataViews["UD02"];

		DateTime currentDay = DateTime.Today; 
		edv.dataView[edv.Row].BeginEdit();
        edv.dataView[edv.Row]["Date01"] = currentDay.AddMonths(1);
		edv.dataView[edv.Row].EndEdit();
	}
}

