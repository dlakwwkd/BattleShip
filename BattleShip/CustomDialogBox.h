#pragma once
#define MAX_TOKEN_NUM		7
#define MAX_STRING_SIZE		59

#define DIALOG_BOX_POS_X	25
#define DIALOG_BOX_POS_Y	22
#define DIALOG_BOX_SIZE_X	62
#define DIALOG_BOX_SIZE_Y	11

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

