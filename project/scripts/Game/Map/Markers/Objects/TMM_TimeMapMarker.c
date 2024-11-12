//  Map Marker Time object class
// Upon creation it gives a map marker an additional time indicating the time it was SCR_AICreateDefendBehaviorCoverQueryProps
modded class SCR_MapMarkerBase
{
	protected string m_sInputText;
	
	// Set marker text with utc time
	override void SetCustomText(string text)
	{
		m_sInputText = text;
		m_sCustomText = text + "\nUTC " + GetUTCTime();
	}
	
	// Set marker text without adding local time if necessary
	void SetCustomText(string text, bool newTime)
	{
		if (!newTime)
		{
			m_sCustomText = text;
		}
	}
	
	// Set the the text that the player inputted
	void SetInputText(string text)
	{
		m_sInputText = text;
	}
	
	// Get the text that the player inputted
	string GetInputText()
	{
		return m_sInputText;
	}
	
	// Get the utc time in hh-mm
	string GetUTCTime()
	{
		string timeString = SCR_DateTimeHelper.GetTimeUTC();
			
		return timeString.Substring(0, 5);
	}
}
