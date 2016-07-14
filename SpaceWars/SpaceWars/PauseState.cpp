//
//  PauseState.cpp
//  SpaceWars
//
//  Created by Erica Ko on 18/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#include "PauseState.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"
#include "Button.hpp"
#include "MusicPlayer.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

PauseState::PauseState(StateStack& stack, Context context, bool letUpdatesThrough)
: State(stack, context)
, mBackgroundSprite()
, mPausedText()
, mGUIContainer()
, mLetUpdatesThrough(letUpdatesThrough)
{
    sf::Font& font = context.fonts->get(Fonts::Main);
    sf::Vector2f windowSize(context.window->getSize());
    
    mPausedText.setFont(font);
    mPausedText.setString("Game Paused");
    mPausedText.setCharacterSize(70);
    centerOrigin(mPausedText);
    mPausedText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
    
    auto returnButton = std::make_shared<GUI::Button>(context);
    returnButton->setPosition(0.5 * windowSize.x - 100, 0.4f * windowSize.y + 75);
    returnButton->setText("Return");
    returnButton->setCallback([this] ()
                              {
                                  requestStackPop();
                              });
    
    auto backToMenuButton = std::make_shared<GUI::Button>(context);
    backToMenuButton->setPosition(0.5 * windowSize.x - 100, 0.4f * windowSize.y + 125);
    backToMenuButton->setText("Back to menu");
    backToMenuButton->setCallback([this] ()
                              {
                                  requestStateClear();
                                  requestStackPush(States::Menu);
                              });
    
    // GUI::Container#pack takes an argument of type Component::Ptr. But we declare returnButton to be a pointer to
    // a type of GUI::Button. So how can we pass returnButton as an argument into the pack method when it expects
    // a different type. The answer is because GUI::Button is a subclass of GUI::Component so C++ allows it.
    mGUIContainer.pack(returnButton);
    mGUIContainer.pack(backToMenuButton);
    
    getContext().music->setPaused(true);
}

PauseState::~PauseState()
{
    getContext().music->setPaused(false);
}

void PauseState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    
    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());
    
    window.draw(backgroundShape);
    window.draw(mPausedText);
    window.draw(mGUIContainer);
}

bool PauseState::update(sf::Time dt)
{
    return mLetUpdatesThrough;
}

bool PauseState::handleEvent(const sf::Event &event)
{
    mGUIContainer.handleEvent(event);
    return false;
}