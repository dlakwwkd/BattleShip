#pragma once
#include "Enums.h"

#define MAX_TOKEN_NUM		6
#define MAX_STRING_SIZE		59

#define DIALOG_BOX_POS_X	25
#define DIALOG_BOX_POS_Y	23
#define DIALOG_BOX_SIZE_X	62
#define DIALOG_BOX_SIZE_Y	10

class CustomDialogBox
{
public:
	CustomDialogBox();
	~CustomDialogBox();

	std::string	GetMessage(){ return msgToken.back(); }

	void InitDialog();
	void InputDialog();
	void PrintDialog();
	void ProcessMessage();
	void InputSystemMessage(HitResult hit, int player);
	void InputSystemMessage(std::string msg);
	void Decoration();

private:
	std::vector<std::string> msgToken;
};

