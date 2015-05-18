#include "datahandler.h"
#include <QDebug>

DataHandler::DataHandler()
{

}

void DataHandler::save(std::string fileName, QList<Place *> places, QList<Transition *> transitions, QList<Arrow *> arrows)
{
    Json::Value root;
    Json::Value p(Json::arrayValue);

    Place *p1 = new Place(23, 45, 0);
    Place *p2 = new Place(34, 4545, 0);
    Place *p3 = new Place(345, 33, 0);

    Json::Value place1;
    place1["id"] = p1->id;
    place1["x"] = p1->x;
    place1["y"] = p1->y;
    place1["liveness"] = p1->liveness;

    Json::Value place2;
    place2["id"] = p2->id;
    place2["x"] = p2->x;
    place2["y"] = p2->y;
    place2["liveness"] = p2->liveness;

    Json::Value place3;
    place3["id"] = p3->id;
    place3["x"] = p3->x;
    place3["y"] = p3->y;
    place3["liveness"] = p3->liveness;

    p.append(place1);
    p.append(place2);
    p.append(place3);

    root["places"] = p;

    delete p1;
    delete p2;
    delete p3;

    std::ofstream saveFile;
    saveFile.open(fileName.c_str());
    saveFile << writer.write(root);
    saveFile.close();
}

void DataHandler::load(std::string fileName, QList<Place *> places, QList<Transition *> transitions, QList<Arrow *> arrows)
{
    Json::Value root;
    std::ifstream loadFile(fileName.c_str());
    bool parsed = reader.parse(loadFile, root, false);
    if (!parsed)
    {
        // report to the user the failure and their locations in the document.
        std::cout  << "Failed to parse configuration\n" << reader.getFormatedErrorMessages();
    }

    std::cout << root;
}
