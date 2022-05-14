modded class RecipeBase
{

	override void PerformRecipe(ItemBase item1, ItemBase item2, PlayerBase player)
	{
		MoreAdminLogs h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
		if (!h_madHelper.IsEnabled()) 
		{
			super.PerformRecipe(item1, item2, player);
			return;
		}
		// don't call super - copy-paste original to get sources original
		if( item1 == NULL || item2 == NULL )
		{
			Error("PerformRecipe: recipe invalid, at least one of the ingredients is NULL");
			Debug.Log("PerformRecipe: at least one of the ingredients is NULL","recipes");
		}
		/*
		m_IngredientsSorted[0] = item1;
		m_IngredientsSorted[1] = item2;
		*/
		
		/*
		Debug.Log("PerformRecipe Ingredient 1: "+ToString(item1.Ptr().GetType()),"recipes");
		Debug.Log("PerformRecipe Ingredient 2: "+ToString(item2.Ptr().GetType()),"recipes");
		*/
		if( CheckRecipe(item1,item2,player) )
		{
			string source1 = h_madHelper.helper().ItemInfo(item1, false, false, false, true);
			string source2 = h_madHelper.helper().ItemInfo(item2, false, false, false, true);
			string sources = "";
			if (source1 != "" && source1 != "null") {
				sources = source1;
			}
			if (source2 != "" && source2 != "null") {
				if (sources != "") sources += ",";
				sources += source2;
			}

			ref array<ItemBase> spawned_objects = new array<ItemBase>;
			SpawnItems(m_IngredientsSorted, player,spawned_objects );
			
			ApplyModificationsResults(m_IngredientsSorted, spawned_objects, NULL, player);
			ApplyModificationsIngredients(m_IngredientsSorted, player);
			ApplySoftSkillsSpecialty(player);
			Do( m_IngredientsSorted, player, spawned_objects, m_Specialty );
			DeleleIngredientsPass();
			
			string resultsStr = "";
			for( int i = 0; i < spawned_objects.Count(); i++ )
			{
				string nextItem = h_madHelper.helper().ItemInfo(spawned_objects[i], false, false, false, true);
				if (i > 0 && nextItem != "" && nextItem != "null") resultsStr += ", ";
				if (nextItem != "" && nextItem != "null") resultsStr += nextItem;
			}
			if (h_madHelper.IsJson()) {
				h_madHelper.PlayerLog(player, "CRAFT_LOG", string.Format("{\"recipe\":\"%1\",\"recipeId\":\"%2\",\"sources\":[%3],\"results\":[%4]}", GetName(), GetID(), sources, resultsStr));
			} else {
				h_madHelper.PlayerLog(player, "CRAFT_LOG", string.Format("<%1::%2> sources=<%3> results=<%4>", GetName(), GetID(), sources, resultsStr));
			}
			
		}
		else
		{
			Debug.Log("CheckRecipe failed on server","recipes");
		}
	}
}