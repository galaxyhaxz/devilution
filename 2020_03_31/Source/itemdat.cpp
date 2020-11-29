#include "diablo.h"

ItemDataStruct AllItemsList[157] =
{
#include "Data/xl_item.cpp"
};

const PLStruct PL_Prefix[84] =
{
#include "Data/xl_ipre.cpp"
};

const PLStruct PL_Suffix[96] =
{
#include "Data/xl_isuf.cpp"
};

const UItemStruct UniqueItemList[91] =
{
#include "Data/xl_uitem.cpp"
};
