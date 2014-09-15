#pragma once
class Menu
{
public:
	static Menu& Instance()
	{
		static Menu instance;
		return instance;
	}
	void	MainMenu();
	void	PlayerMenu();
	void	MenuPlatform(std::vector<std::string>& menuList, int menuType);
	int		MenuMove(int& curMenu, int maxMenuNum);

private:
	Menu();

};

