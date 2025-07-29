#pragma once
#include "GameObject.h"

class UiButton;
class YesNoPopupUI;

class PopupWindowUI :
	public GameObject
{
protected:
	sf::Sprite sprite;	
	
	std::vector<UiButton*> startbut;	

	int currentPage = 0;

	sf::FloatRect spr;
	UiButton* Local;
	UiButton* Editor;
	UiButton* Exit;

	UiButton* Rightbut;
	UiButton* Leftbut;
	UiButton* closebut;

	bool enterLock = true;
	YesNoPopupUI* yesno;

	


public:
	bool drawon = false;
	PopupWindowUI(const std::string name = "");
	~PopupWindowUI() override = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;

	YesNoPopupUI* GetYesNoPopup() const { return yesno; }


	void Init() override;

	void Release() override;

	void Reset() override;

	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;

};

