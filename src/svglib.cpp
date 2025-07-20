#include "svglib.hpp"

sl::fileinfo sl::fileLoader(char * path)
{
    //std::cout<<"keep in mind that only PATH element is going to be implemented\n";
    XMLDocument file;
    if (file.LoadFile(path)!=XML_SUCCESS){throw std::invalid_argument("file inavalible: unaccessable");}
    XMLElement *root = file.RootElement();
    if (root==nullptr){throw std::invalid_argument("file inavalible: not xml formate");}
    else if (*root->Value()!=*"svg"){throw std::invalid_argument("file inavalible: can't find svg as xml root");}
    else {
        fileinfo returns;//[(list of path,list transform)..]
        std::stringstream transform;
        std::stringstream path;
        pathSearch(root,returns);
        if (returns.empty()){throw std::invalid_argument("file inavalible: no path element is found");}
        return returns;
    }
}



void sl::pathSearch(XMLElement *root,fileinfo& returns,std::string uptransform)
{
    XMLElement *element=root->FirstChildElement();
    std::string temptrans;
    while (element!=nullptr){
        temptrans=uptransform;
        if (*element->Value()==*"path"){
            std::string tempCommand;
            transformExtract(element,temptrans);
            filepath path;
            std::stringstream transsplit(temptrans);
            while(std::getline(transsplit,tempCommand,')')){
                path[0].push_back(tempCommand+")");
            }
            const XMLAttribute *attr=element->FindAttribute("d");
            std::stringstream pathtext(attr->Value());
            while(std::getline(pathtext,tempCommand,'z')){
                path[1].push_back(tempCommand+"z");
            }returns.push_back(path);
        }
        else if (*element->Value()==*"g"){
            transformExtract(element,temptrans);
            pathSearch(element,returns,temptrans);
        }
        element=element->NextSiblingElement();
    }
}

void sl::transformExtract(XMLElement * element,std::string& transform)
{
    const XMLAttribute *attr=element->FindAttribute("transform");
    if (attr==nullptr){return;}
    //if (!transform.empty()){transform+=" ";}
    transform+=attr->Value();
}

void sl::printFile(fileinfo info){
    // typedef std::vector<std::array<std::vector<std::string>,2>> fileinfo;
    std::string label[2]={"transform","path"};
    for (const auto &i:info){// loop through pathes
        for (int j=0;j<2;j++){// loop through [path, transform]
            std::cout<<label[j]<<": \n";
            for (const auto &k:i[j]){// loop through commands in one path/transform
                std::cout<<"\t"<<k<<"\n";
            }
        }
    }std::cout<<"\n";
}



sl::svginfo sl::file2vector(fileinfo file)
{
    /*ok i spend some time trying to understand how those datatypes i created my self works
    so in filepath, you can imagen it as a pair in a python dict, 
    where transform is the key, and they all apply to those listed pathes
    imagen the list of transform as one item, and it links to multiply pathes
    and in this function, i'm gonna relink each path all with one transform
    probably not the most optinal but i'm really tired from designing another new datatype*/

    // typedef std::array<std::vector<std::string>,2> filepath
    svginfo returns;
    for (const filepath &i:file){
        std::vector<std::string> pathes=i[1];
        transforminfo tranCommands;trans2vector(tranCommands,i[0]);
        pathinfo pathCommands;
        for (const std::string &i:pathes){
            path2vector(pathCommands,i);
            returns.push_back({pathCommands,tranCommands});
            pathCommands.clear();
        }
    }
    return returns;
}


void sl::path2vector(pathinfo& returns, std::string info)
{
    char tempkey=0;
    std::stringstream tempitem;
    std::vector<double> tempitems;
    bool exitLoop=false;
    for (char i:info){
        switch (i)
        {
        case ' ':case ',':case '\n':case '\t':// see space meaning a number ended and the next number is not nega
            if (tempitem.str().empty()){break;}
            tempitems.push_back(std::stod(tempitem.str()));
            tempitem.str("");
            break;
        case '-':// see nega meaning a number ended and the next number is not nega
            if (tempitem.str().empty()){tempitem<<'-';break;}
            tempitems.push_back(std::stod(tempitem.str()));
            tempitem.str("");
            tempitem<<'-';
            break;
        default:// means its either a letter or part of a number
            if (std::isalpha(i)){// if letter
                if (!tempitem.str().empty()){
                    tempitems.push_back(std::stod(tempitem.str()));
                    tempitem.str("");
                }// ^2 updates the current existing number if any
                if (tempkey && tempitems.size()){// in case if it is the first letter
                    returns.push_back({tempkey,tempitems});
                }tempkey=i;tempitems.clear();// clear, obviously
            }else {tempitem<<i;}// if it is part of a number
            break;
        }
    }
}

void sl::trans2vector(transforminfo& returns, std::vector<std::string> commands)
{
    for (std::string info:commands)
    {// now info is one single command
        std::stringstream infostream(info);// one command
        std::stringstream tempitem;// store number
        std::vector<double> tempitems;// store numbers
        std::string key;
        std::getline(infostream,key,'('); // removes/extracts key, expected format: "num,num,num)"
        key.erase(std::remove(key.begin(), key.end(), ' '), key.end());
        std::stringstream remaind;remaind<<infostream.rdbuf();// implement the change
        bool exitLoop=false;
        for (char i:remaind.str()){
            if (exitLoop){break;}
            switch (i)
            {
            case ' ':case ',':case '\n':case '\t':// see space meaning a number ended and the next number is not nega
                if (tempitem.str().empty()){break;}
                tempitems.push_back(std::stod(tempitem.str()));
                tempitem.str("");
                break;
            case '-':// see nega meaning a number ended and the next number is not nega
                if (tempitem.str().empty()){tempitem<<'-';break;}
                tempitems.push_back(std::stod(tempitem.str()));
                tempitem.str("");
                tempitem<<'-';
                break;
            case ')':
                if (!tempitem.str().empty())
                {tempitems.push_back(std::stod(tempitem.str()));}
                exitLoop=true;
            default:// means its either a letter or part of a number
                tempitem<<i;// if it is part of a number
                break;
            }
        }returns.push_back(std::pair(key,tempitems));
    }   
}


std::vector<sl::pointgraph> sl::svg2points(svginfo fileinfos)
{
    std::vector<pointgraph> returns;
    point *travelPtr=nullptr;
    for (int i=0;i<fileinfos.size();i++){
        std::vector<pointgraph> newParts=getPointGraph(fileinfos[i],travelPtr);
        returns.insert(returns.end(),newParts.begin(),newParts.end());
    }

    return returns;
}

std::array<double, 7> sl::getrepoinfo(std::vector<pointgraph> parts,double width)
{
    double xmax=0,xmin=0,ymax=0,ymin=0;
    for (pointgraph points:parts){
        for (point i:points){
            if (i[0]>xmax){xmax=i[0];} else if (i[0]<xmin){xmin=i[0];}
            if (i[1]>ymax){ymax=i[1];} else if (i[1]<ymin){ymin=i[1];}
        }
    }
    double xlargest=std::abs(xmax)-std::abs(xmin),ylargest=std::abs(ymax)-std::abs(ymin);
    double resoRate=width/std::max({xlargest,ylargest})*0.8;
    return {xmax,xmin,ymax,ymin,xlargest,ylargest,resoRate};
}

std::vector<sl::pointgraph> sl::resizeRepo(std::vector<pointgraph> parts,double xmin,double ymin,double resRate)
{// for this and some other functions, name "parts" is used for original, cus i copied the code and i'm too lazy to change
    std::vector<pointgraph> returns;
    for (pointgraph points:parts){
        pointgraph temp;
        for (std::array<double,2>i:points){
            temp.push_back({((i[0]-xmin)*resRate),((i[1]-ymin)*resRate)});
        }returns.push_back(temp);
    }return returns;
}

std::vector<sl::pointgraph> sl::getPointGraph(std::pair<pathinfo,transforminfo> info, point* globalPtr)
{
    std::vector<sl::pointgraph> returns;
    pointgraph traveledPtrs;
    if (globalPtr!=nullptr){traveledPtrs={*globalPtr};}
    point travelPtr;
    // ^store every single point it travelled on, be returned
    pointgraph temp;// temp, ig
    std::string charList="m hv qsct l ea";
    for (std::pair<char,std::vector<double>> i:info.first)
    {
        if (!traveledPtrs.empty()){travelPtr=traveledPtrs[traveledPtrs.size()-1];}
        // ^update point to the latest
        if ((std::string::npos==charList.find(std::tolower(i.first)))){
            std::cout << "I see you got a "<<i.first<<", this is not ok in a svg file bruh. \n";
            std::cout<<"we are simply not gonna count it, but just saying\n"; 
            continue;
        }// ^ if the key isn't in the avalible letters
        // ^ now all code below can assume that the letter is avalible
        bool relitive=false;
        if(std::islower(i.first)){relitive=true;}
        i.first=std::tolower(i.first);
        // ^ check the difference, then unite
        if (i.first == 'm'){
            if(traveledPtrs.size()>1){returns.push_back(traveledPtrs);}
            traveledPtrs.clear();
            point destingation={i.second[0],i.second[1]};
            if (relitive){destingation[0]+=travelPtr[0];destingation[1]+=travelPtr[1];}
            traveledPtrs.push_back(destingation);
            continue;
        }
        else if (i.first == 'a'||i.first=='e'){
            temp=eagpg(i.second,travelPtr,relitive,&travelPtr);
            traveledPtrs.insert(traveledPtrs.end(),temp.begin(),temp.end());
            continue;
        }
        if (i.first == 'h'){
            i.second.push_back(0);
            if (!relitive){i.second[1]=travelPtr[1];}
        }
        else if (i.first == 'v'){
            i.second.insert(i.second.begin(),0);
            if (!relitive){i.second[0]=travelPtr[0];}
        }
        temp=array2Ptr(i.second);
        if (relitive){
            for (int i =0;i<temp.size();i++)
            {temp[i][0]+=travelPtr[0];temp[i][1]+=travelPtr[1];}
        }
        temp.insert(temp.begin(),travelPtr);
        temp=bcgpg(temp);
        traveledPtrs.insert(traveledPtrs.end(),temp.begin(),temp.end());
    }
    if (globalPtr==nullptr){globalPtr=new point;}
    *globalPtr=traveledPtrs[traveledPtrs.size()-1];
    returns.push_back(traveledPtrs);
    return returns;
}

/*sl::pointgraph sl::applyTran(pointgraph points,transforminfo info)
{
    for (std::pair<std::string,std::vector<double>> command:info){
        for (point &i:points){
            i=st::apply(command.first,command.second,i);
        }
    }
    return points;
}*/

sl::pointgraph sl::bcgpg(pointgraph curveInfo)
{// stands for bezier curve get point graph
    // curvePtr=applyTran(curvePtr,tranInfo); // tried apply transform, failed
    pointgraph returns;
    float interval=0.001;
    for (float i=interval;i<1+interval;i+=interval){
        returns.push_back(dca(curveInfo,i));
    }
    return returns;
}

sl::point sl::dca(pointgraph points, float t)
{// stands for de casteljau algorithm, basicly recursive lerp, aka how bezier curve works
    if (points.size()==1){
        return points[0];
    }
    pointgraph nextLev;
    for (int i=0;i<points.size()-1;i++){
        point p0=points[i];point p1=points[i+1];
        nextLev.push_back(lerp2d(p0,p1,t));
    }
    return dca(nextLev,t);
}

sl::pointgraph sl::eagpg(std::vector<double> curveInfo,point startPtr,bool relitive,point* travel)
{// stands for Elliptical Arc get point graph
    throw std::invalid_argument("elliptical arc detected: this our problem, we didn't includ it because its fucking hard to code and not one use it");
}

sl::pointgraph sl::simplify(pointgraph old,double minLen,double maxLen,double minAngle)
{
    pointgraph returns={old[0]};
    for (int i=1;i<old.size();i++){
        point last=returns[returns.size()-1];
        point current=old[i];
        point next=(i==old.size()-1) ? old[0] : old[i+1];

        double dx=last[0]-current[0],dy=last[1]-current[1];
        double distant=std::sqrt(dx*dx+dy*dy);
        if (minLen>=distant){continue;}
        if (distant>=maxLen){returns.push_back(old[i]);continue;}

        double dx1=current[0]-next[0],dy1=current[1]-next[1];
        double angle=std::abs(std::tan(dx1/dy1)*toDegree-std::tan(dx/dy)*toDegree);
        if (angle>180-minAngle){returns.push_back(old[i]);continue;}
    }returns.push_back(old[old.size()-1]);
    return returns;
}


void sl::printInfo(svginfo info){
    for (auto path:info){
        for (const auto &i:path.first)
        {
            std::cout<<i.first<<": ";
            for (double j:i.second)
            {
                std::cout<<j<<"\t";
            }
            std::cout<<std::endl;
        }
        for (const auto &i:path.second)
        {
            std::cout<<i.first<<": ";
            for (double j:i.second)
            {
                std::cout<<j<<"\t";
            }
            std::cout<<std::endl;
        }std::cout<<std::endl;
    }
}

sl::point sl::lerp2d(point p0,point p1, float t)
{
    point p;
    for (int i=0;i<2;i++){
        p[i]=(1.-t)*p0[i]+t*p1[i];
    }return p;
}
sl::pointgraph sl::array2Ptr(std::vector<double> array)
{
    if (array.size()%2){throw std::invalid_argument("file inavalible: one of the commands in uneven size");}
    pointgraph ptrs;
    point tempPtr;
    for (int i=0;i<array.size();i+=2){
        tempPtr[0]=array[i];tempPtr[1]=array[i+1];
        ptrs.push_back(tempPtr);
    }return ptrs;
}