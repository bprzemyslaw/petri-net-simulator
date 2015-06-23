#include "simulationengine.h"

SimulationEngine &SimulationEngine::getInstance()
{
    static SimulationEngine instance;
    return instance;
}

SimulationEngine::SimulationEngine(): QObject(0) {timer.start();}
SimulationEngine::SimulationEngine(const SimulationEngine &engineToCopy): QObject(0) {Q_UNUSED(engineToCopy)}

SimulationEngine::~SimulationEngine()
{
    timer.setRunning(false);
    timer.quit();
    timer.wait(10000);
}

void SimulationEngine::beginSimulation() {
    this->activateTransitions();
}

void SimulationEngine::endSimulation() {
    this->deactivateAllTransitions();
}

void SimulationEngine::activateTransitions() {
    for (Element *transition: *this->transitions) {
        bool active = true;
        QList<Arrow *> connectedArrows = this->arrowsForElement(transition);
        for (Arrow *arrow: connectedArrows) {
            if (arrow->fromPlaceToTransition) {
                Place *place = (Place *)(arrow->place);
                bool notEnoughTokens = place->tokenCount() < arrow->weight();
                if (notEnoughTokens) {
                    active = false;
                    goto escape;
                }
            }
        }
escape:
        transition->setActive(active);
    }
}

void SimulationEngine::deactivateAllTransitions() {
    for (Element *transition: *this->transitions) {
        transition->setActive(false);
    }
}

QList<Arrow *> SimulationEngine::arrowsForElement(Element *element) {
    QList<Arrow *> connectedArrows;
    for (Arrow *arrow: *(this->arrows)) {
        if (arrow->place == element || arrow->transition == element) {
            connectedArrows.append(arrow);
        }
    }
    return connectedArrows;
}

void SimulationEngine::executeTransition(Element *transition) {
    // TODO:
    QList<Arrow *> associatedArrows = SimulationEngine::arrowsForElement(transition);

    for (Arrow *arrow: associatedArrows){
        Place *place = qobject_cast<Place*>(arrow->place);
        if (arrow->fromPlaceToTransition){
            place->setTokenCount(place->tokenCount() - arrow->weight());
        }
        else {
            place->setTokenCount(place->tokenCount() + arrow->weight());
        }
    }
    this->activateTransitions();
}

QList<State *> SimulationEngine::generateReachabilityStates() {
    QList<State *> states;
    this->attachChildrenStates(this->getInitialState(), &states);
    return states;
}

QList<State *> SimulationEngine::getFakeStates()
{
    return generateReachabilityStates();
}

// TODO Zmienić wartość zwracaną na coś bardziej przejrzystego
int SimulationEngine::livenessForTransition(Transition *transition)
{
    QList<State *> states = this->getFakeStates();
    int occurenceNumber = 0;
    for (State *state: states){
        for (State::StateConnection stateConnection: state->outgoingConnections){
            if (transition == stateConnection.transition){
                ++occurenceNumber;
            }
        }
    }
    return occurenceNumber;
}

int SimulationEngine::tokenSumForState(State *state) const
{
    int sum = 0;
    for (int tokens : state->tokenCounts) {
        sum += tokens;
    }
    return sum;
}

QStringList SimulationEngine::generateLivenessReport()
{
    QStringList reportList;
    bool alive = true;
    reportList << "<b>Transitions:</b>";

    for (Element *transition: *(this->transitions)){
        Transition *examinedTransition = qobject_cast<Transition *>(transition);
        int occurenceNumber = this->livenessForTransition(examinedTransition);
        QString transitionNumber = "T"+QString::number(transition->number());
        QString liveness = occurenceNumber? "Alive" : "Dead";
        if (!occurenceNumber){
            alive = false;
        }
        reportList << QString(transitionNumber + " : " + liveness);
    }

    QString liveness = alive? "Alive" : "Dead";
    reportList << QString("<b>Net liveness:</b> " + liveness);
    return reportList;
}

QString SimulationEngine::generateConservationReport()
{
    QList<State *> states = this->getFakeStates();
    bool conservative = true;
    int tokenSum = this->tokenSumForState(states.first());

    for (State *state: states){
        int sumForState = this->tokenSumForState(state);
        if (tokenSum != sumForState){
            conservative = false;
            break;
        }
    }
    QString conservativeString = conservative? "Conservative" : "Non-conservative";
    QString conservativeReport = "<b>Net conservation:</b> "+ conservativeString;
    return conservativeReport;
}

void SimulationEngine::attachChildrenStates(State *currentState, QList<State *> *states) { // TODO: Refactor name
    states->append(currentState);
    static int level = 0;
    if (level == 10) {
        return;
    }
    ++level;
    QList<Element *> activeTransitions = this->activeTransitionsForSimulatedState(currentState);
    for (Element *transition: activeTransitions) {
        State *nextState = this->stateAfterTransitionFromState(currentState, transition);
        nextState->level = level;
        bool duplicate = false;
        for (State *state: *states) {
            if (state->tokenCounts == nextState->tokenCounts) {
                duplicate = true;
                delete nextState;
                currentState->outgoingConnections.append(State::StateConnection(state, transition));
                break;
            }
        }
        if (!duplicate) {
            currentState->outgoingConnections.append(State::StateConnection(nextState, transition));
            this->attachChildrenStates(nextState, states);
        }
    }
    --level;
}

State * SimulationEngine::stateAfterTransitionFromState(State *state, Element *transition) {
    State *newState = new State(*state);
    QList<Arrow *> connectedArrows = this->arrowsForElement(transition);
    for (Arrow *arrow: connectedArrows) {
        int placeNumber = arrow->place->number();
        if (arrow->fromPlaceToTransition) {
            newState->tokenCounts[placeNumber] -= arrow->weight();
        }
        else {
            newState->tokenCounts[placeNumber] += arrow->weight();
        }
    }
    return newState;
}

QList<Element *> SimulationEngine::activeTransitionsForSimulatedState(State *state) {
    QList<Element *> activeTransitions;
    for (Element *transition: *this->transitions) {
        bool active = true;
        QList<Arrow *> connectedArrows = this->arrowsForElement(transition);
        for (Arrow *arrow: connectedArrows) {
            if (arrow->fromPlaceToTransition) {
                int placeTokenCount = state->tokenCounts[arrow->place->number()];
                bool notEnoughTokens = placeTokenCount < arrow->weight();
                if (notEnoughTokens) {
                    active = false;
                    break;
                }
            }
        }
        if (active) {
            activeTransitions.append(transition);
        }
    }
    return activeTransitions;
}

State * SimulationEngine::getInitialState() {
    State *newState = new State(*this->places);
    newState->level = 0;
    return newState;
}
