#ifndef SCGRAPH_UI_POOL_HH
#define SCGRAPH_UI_POOL_HH

#include <QtUiTools/QUiLoader>

#include <vector>


class UIPool {
	std::vector<QUiLoader> _ui_loaders;
};


#endif

