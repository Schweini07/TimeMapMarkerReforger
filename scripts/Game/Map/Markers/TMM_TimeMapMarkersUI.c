modded class SCR_MapMarkersUI
{
	override protected void OnDragEnd(Widget widget, bool wasDragged)
	{		
		SCR_MapMarkerBase marker = m_MarkerMgr.GetMarkerByWidget(widget);
		if (marker)
		{
			SCR_EMapMarkerType type = marker.GetType();
			
			if (!IsOwnedMarker(marker) || (type != SCR_EMapMarkerType.PLACED_CUSTOM && type != SCR_EMapMarkerType.PLACED_MILITARY))
				return;
			
			marker.SetDragged(false);
			
			if (!wasDragged)
				return;
			
			vector pos = FrameSlot.GetPos(widget);

			float wX, wY;
			m_MapEntity.ScreenToWorld(GetGame().GetWorkspace().DPIScale(pos[0]), GetGame().GetWorkspace().DPIScale(pos[1]), wX, wY);
			
			marker.SetWorldPos(wX, wY); // set the old marker pos here as well because of the delay it takes between client and server to delete it
			
			SCR_MapMarkerBase markerNew = new SCR_MapMarkerBase();
			markerNew.SetType(marker.GetType());
			markerNew.SetWorldPos(wX, wY);
			markerNew.SetInputText(marker.GetInputText());
			markerNew.SetCustomText(marker.GetCustomText(), false);
			markerNew.SetMarkerFactionFlags(marker.GetMarkerFactionFlags());
			
			if (type == SCR_EMapMarkerType.PLACED_CUSTOM)
			{
				markerNew.SetColorEntry(marker.GetColorEntry());
				markerNew.SetIconEntry(marker.GetIconEntry());
				markerNew.SetRotation(marker.GetRotation());
			}
			else 
			{
				markerNew.SetFlags(marker.GetFlags());
				markerNew.SetMarkerConfigID(marker.GetMarkerConfigID()); // combination of faction and dimension id
			}
			
			int markerID = marker.GetMarkerID();
			RemoveOwnedMarker(marker);
			
			bool isLocal = markerID == -1;
			m_MarkerMgr.InsertStaticMarker(markerNew, isLocal);
		
			m_OnCustomMarkerPlaced.Invoke(wX, wY, isLocal);
		}
	}
	
	override protected void OnInputMapSelect(float value, EActionTrigger reason)
	{
		if ((m_CursorModule.GetCursorState() & SCR_MapCursorModule.STATE_POPUP_RESTRICTED) != 0)
			return;

		array<Widget> widgets = SCR_MapCursorModule.GetMapWidgetsUnderCursor();
		
		SCR_MapMarkerWidgetComponent markerComp;
		foreach ( Widget widget : widgets )
		{
			markerComp = SCR_MapMarkerWidgetComponent.Cast(widget.FindHandler(SCR_MapMarkerWidgetComponent));	
			if (!markerComp)
				continue;
						
			SCR_MapMarkerBase marker = m_MarkerMgr.GetMarkerByWidget(widget);
			if (!marker)
				continue;
		
			m_EditedMarker = marker;
			SCR_EMapMarkerType type = marker.GetType();
			
			if (IsOwnedMarker(marker) && (type == SCR_EMapMarkerType.PLACED_CUSTOM || type == SCR_EMapMarkerType.PLACED_MILITARY))
			{	
				int wPos[2];
				float screenX, screenY;
				marker.GetWorldPos(wPos);
				m_MapEntity.WorldToScreen(wPos[0], wPos[1], screenX, screenY);
				m_MapEntity.PanSmooth(screenX, screenY, 0.1);
								
				if (type == SCR_EMapMarkerType.PLACED_CUSTOM )
				{
					if (!m_PlacedMarkerConfig)
						return;
					
					CreateMarkerEditDialog(true, m_PlacedMarkerConfig.GetIconCategoryID(marker.GetIconEntry()), marker.GetIconEntry(), marker.GetColorEntry());
					
					OnEditBoxTextChanged(marker.GetInputText());
					m_SliderComp.SetValue(marker.GetRotation());
					m_EditBoxComp.SetValue(marker.GetInputText());
				} 
				else 
				{
					if (!m_MilitaryMarkerConfig)
						return;
					
					CreateMilitaryMarkerEditDialog(true, marker.GetMarkerConfigID() % SCR_MapMarkerEntryMilitary.FACTION_DETERMINATOR, marker.GetMarkerConfigID() * SCR_MapMarkerEntryMilitary.DIMENSION_DETERMINATOR);
					OnEditBoxTextChanged(marker.GetInputText());
					m_EditBoxComp.SetValue(marker.GetInputText());
					
					int markerFlags = marker.GetFlags();
					bool secondType;
					
					array<ref SCR_MarkerMilitaryType> milTypes = m_MilitaryMarkerConfig.GetMilitaryTypes();
					foreach (int i, SCR_MarkerMilitaryType milType : milTypes)
					{
						if (markerFlags & milType.GetType())
						{
							if (secondType)
							{
								m_ComboBoxComp2.SetCurrentItem(i+1);	// +1 is here and below since we add a NONE entry to the combo boxes during creation
								OnComboBoxChangedB(m_ComboBoxComp2, i+1);
								break;
							}
							else 
							{
								m_ComboBoxComp1.SetCurrentItem(i+1);
								OnComboBoxChangedA(m_ComboBoxComp1, i+1);					
								if (markerFlags == milType.GetType())	// return if singular flag
									break;
								
								secondType = true;
							}
						}
					}
				}
				
				marker.SetVisible(false);												
				
				break;
			}
		}
	}
}