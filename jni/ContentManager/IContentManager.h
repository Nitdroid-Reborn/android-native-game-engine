#ifndef ICONTENTMANAGER_H
#define ICONTENTMANAGER_H


class IContentManager {
public:
    IContentManager(){}
    virtual ~IContentManager(){}

    virtual bool Initialize()=0;
    virtual bool Release()=0;


    static IContentManager* get(){return singleton;}

protected:
    static IContentManager* singleton;
};

#endif // ICONTENTMANAGER_H
