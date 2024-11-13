modded class SCR_MapMarkerManagerComponent
{
	override event protected bool RplLoad(ScriptBitReader reader)
	{
		int count;
		reader.ReadInt(count);
		if (count == 0)
			return true;
		
		int posX, posY, markerID, markerOwnerID, flags, markerConfigID, factionFlags, markerType, colorID, iconID, rotation;
		string customText;
		SCR_MapMarkerBase marker;
		
		for (int i; i < count; i++)
		{	
				
			reader.ReadInt(posX);
			reader.ReadInt(posY);
			reader.ReadInt(markerID);
			reader.ReadInt(markerOwnerID);
			reader.ReadInt(flags);
			reader.ReadInt(markerConfigID);
			reader.ReadInt(factionFlags);
			reader.Read(rotation, 16);
			reader.Read(markerType, 8);
			reader.Read(colorID, 8);
			reader.Read(iconID, 16);
			reader.ReadString(customText);	
			
			marker = new SCR_MapMarkerBase();
			marker.SetType(markerType);
			marker.SetWorldPos(posX, posY);
			marker.SetMarkerID(markerID);
			marker.SetMarkerOwnerID(markerOwnerID);
			marker.SetFlags(flags);
			marker.SetMarkerConfigID(markerConfigID);
			marker.SetMarkerFactionFlags(factionFlags);
			marker.SetRotation(rotation);
			marker.SetColorEntry(colorID);
			marker.SetIconEntry(iconID);
			marker.SetCustomText(customText, false);
						
			m_aStaticMarkers.Insert(marker);
		}
		
		return true;
	}
}