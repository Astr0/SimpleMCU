/*
 * utils.h
 *
 * Created: 03.02.2013 22:11:13
 *  Author: astr0
 */ 

#pragma once


#ifndef UTILS_H_
#define UTILS_H_

#define SMCU_WRAPVARIABLE(CLASSNAME, VARNAME)\
struct CLASSNAME\
{\
static decltype(VARNAME)& value(){return VARNAME;}\
};



#endif /* UTILS_H_ */