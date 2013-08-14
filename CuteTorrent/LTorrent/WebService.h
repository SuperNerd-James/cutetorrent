#ifndef WEB_SVC_H
#define WEB_SVC_H

#include <QObject>
#include "http/httplistener.h"
#include "RequestMapper.h"
class WebService : QObject
{
	Q_OBJECT
private:
	HttpListener* listener;
	RequestMapper* mapper;
public:
	WebService(void);
	void Start();
	~WebService(void);
};
#endif
