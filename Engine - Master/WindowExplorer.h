#pragma once
#include "Window.h"
class WindowExplorer : public Window
{
public:
	WindowExplorer(const char* name);
	~WindowExplorer();

	void Draw() override;
	bool IsFocused() const;
	void DrawTreeNode(const char* name, bool isLeaf);
	void ClickBehaviour(const char* name);

public:
	bool		focus = false;
	std::string itemSelected;
};

