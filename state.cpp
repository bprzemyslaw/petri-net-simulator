#include "state.h"
#include "ui_state.h"

State::State(QList<Element *> &places): QWidget(0), ui(0) {
    this->tokenCounts.resize(places.count());
    for (Element *place: places) {
        this->tokenCounts[place->number()] = ((Place *)place)->tokenCount();
    }
}

State::State(const State &state): QWidget(0), ui(0) {
    this->tokenCounts = state.tokenCounts;
}

State::~State() {
    if (ui) {
        delete ui;
    }
}

bool State::operator==(const State &stateToCompareTo) {
    return this->tokenCounts == stateToCompareTo.tokenCounts;
}

void State::setupUi(QWidget *parent)
{
    this->setParent(parent);
    ui = new Ui::State;
    ui->setupUi(this);
    this->show();
    QString text = "(";
    for (int tokenCount: this->tokenCounts) {
        text += QString::number(tokenCount) + ",";
    }
    text += ")";
    ui->tokenCountsLabel->setText(text);
    this->adjustSize();
}
