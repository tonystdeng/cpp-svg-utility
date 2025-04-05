#include "view.hpp"


std::vector<sl::pointgraph> getPointgraph(char* path);
sl::point apply(double rotation,sl::point ft);
Game::Game(char * path)
{
    window=new sf::RenderWindow(sf::VideoMode({width,height}),"fps: ");
    window->setFramerateLimit(60);
    sl::fileinfo filetext=sl::fileLoader(path);
    //sl::printFile(filetext); // testing
    fileinfo=sl::file2vector(filetext);
    //sl::printInfo(fileinfo); // testing
    std::vector<sl::pointgraph> parts=sl::svg2points(fileinfo);

    std::array<double, 7> repoinfo=sl::getrepoinfo(parts,std::min({width,height}));
    double xmax=repoinfo[0],xmin=repoinfo[1],ymax=repoinfo[2],ymin=repoinfo[3],resRate=repoinfo[6];
    double reposition=std::min({width,height})*0.1;

    parts=sl::resizeRepo(parts,xmin-reposition,ymin-reposition,resRate);
    //*/std::vector<sl::pointgraph> parts=getPointgraph(path);
    sf::VertexArray svgshape{sf::PrimitiveType::LineStrip},simplified{sf::PrimitiveType::Points};
    for (sl::pointgraph points:parts){
        for (std::array<double,2>i:points){svgshape.append(sf::Vertex{sf::Vector2f{(float)i[0],(float)i[1]}});}
        svgshapes.push_back(svgshape);  svgshape.clear();

        points=sl::simplify(points);
        //for (int i=0;i<points.size();i++){std::cout<<points[i][0]<<" "<<points[i][1]<<"\n";}
        for (std::array<double,2>i:points){simplified.append(sf::Vertex{sf::Vector2f{(float)i[0]+hw,(float)i[1]}});}
        simplifieds.push_back(simplified);  simplified.clear();
    }
    dividLine.setSize(sf::Vector2f(width/256,height));
    dividLine.setPosition({hw-dividLine.getSize().x,0});
    dividLine.setFillColor({255,63,63});
}

std::vector<sl::pointgraph> getPointgraph(char* path)
{// testing for another project
    std::vector<sl::pointgraph> info=sl::svg2points(sl::file2vector(sl::fileLoader(path)));// W stack?
    for (sl::pointgraph &i:info){
        i=sl::simplify(i);
    }
    std::array<double, 7> repoinfo=sl::getrepoinfo(info);
    double xmin=repoinfo[1],ymin=repoinfo[3],xl=repoinfo[4],yl=repoinfo[5];
    info=sl::resizeRepo(info,xmin+xl/2,ymin+yl/2);// en
    for (sl::pointgraph &i:info){
        for (sl::point &j:i){
            j=apply(M_PI,j);
        }
    }
    return info;
}
sl::point apply(double rotation,sl::point ft)
{
    double newx=ft[0]*std::cos(rotation) - ft[1]*std::sin(rotation);
    double newy=ft[0]*std::sin(rotation) + ft[1]*std::cos(rotation);
    return {newx,newy};
}
Game::~Game()
{
    delete window;
}

void Game::main()
{
    if (checkEvent()){return;}
    updatefps();
    window->clear(sf::Color::Black);
    common();
    for (int i=0;i<svgshapes.size();i++){
        window->draw(svgshapes[i]);
        window->draw(simplifieds[i]);
    }
    window->display();
}

void Game::common()
{
    window->draw(dividLine);
}

bool Game::checkEvent()
{
    while (const std::optional event = window->pollEvent()){
        if (event->getIf<sf::Event::Closed>()){
            window->close();
            return true;}
    }return false;
}

void Game::updatefps()
{
    elapsed=clock.restart();
    fpsupdateCountdown+=elapsed;
    if (fpsupdateCountdown>fpsupdateInterval)
    {
        fps = 1./elapsed.asSeconds();
        fpsloader<<fps;
        window->setTitle("fps: "+fpsloader.str());
        fpsloader.str("");fpsloader.clear();
        fpsupdateCountdown-=fpsupdateInterval;
    }
}
