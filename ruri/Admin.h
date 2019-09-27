#pragma once

void HandleAdmin(_GetParams&& Params, _Con s){
	

	const std::string_view PageName = Params.get<WSTI("p")>();


	if (PageName == "lb"){

		const u32 Mode = al_clamp(StringToNum(u32, Params.get<WSTI("m")>()),(u32)0,(u32)GM_MAX);

		std::string res = "<html><table><tr><td>Rank</td><td>Name</td><td>pp</td></tr>";
		
		std::shared_lock L(RankList[Mode]);

		auto& List = RankList[Mode].List;

		for (size_t i(0), size(List.size()); i < size; i++)
			res += "<tr><td>" + std::to_string(i + 1) + "</td><td>" + GetUsernameFromCache(List[i].ID) + "</td><td>" + std::to_string(List[i].PP) + "</td></tr>";

		res += "</table></html>";

		s.SendData(ConstructResponse(200, Empty_Headers,res));
	}


}