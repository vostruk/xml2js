#include "XMLnode.h"
#include <iterator>

XMLnode::XMLnode(string name, bool text)
{
	Nazwa = name;
	is_text = text;
}

XMLnode::XMLnode(string name, list<pair<string, string>> attribs, list<XMLnode*> childr, list<string> texts)
{
	Nazwa = name;
	XMLnode* node =NULL;
	
	 if (attribs.empty() && childr.empty()){
			if (!texts.empty()) {//only 1 text
				childElements.push_back(pair<bool, XMLnode*>(false, new XMLnode(texts.front(), true)));
			}
		}
	 else{ //some arguments or children and some text
		 for (auto i = attribs.begin(); i != attribs.end(); ++i)
		 {
			 node = new XMLnode("@" + i->first);
			 node->childElements.push_back(pair<bool, XMLnode*>(false, new XMLnode(i->second, true)));
			 childElements.push_back(pair<bool, XMLnode*>(false, node));
		 }
		 for (auto i = childr.begin(); i != childr.end(); ++i)
			 childElements.push_back(pair<bool, XMLnode*>(false, *i));

		 for (auto i = texts.begin(); i != texts.end(); ++i){
			 node = new XMLnode("#text");
			 node->childElements.push_back(pair<bool, XMLnode*>(false, new XMLnode(*i, true)));
			 childElements.push_back(pair<bool, XMLnode*>(false, node));
		 }
	 }
}


XMLnode::~XMLnode()
{
	//delete everything using recursion
	del(childElements);
}
void XMLnode::del(list <pair<bool, XMLnode*>> e){
	for (auto i = e.begin(); i != e.end(); i++)
	{
		if(!i->second->childElements.empty()) del(i->second->childElements);
		delete (i->second);
	}
}
void XMLnode::drawTree(int level, ostream & out)
{
	out << '\n';
	for (int i = 0; i < level; i++) out << "|     ";
	out << "|____" << Nazwa;
	if (is_text) out << '~';
	for (auto i = childElements.begin(); i != childElements.end(); i++) i->second->drawTree(level + 1, out);
}

void XMLnode::writeMe2JSON(JSsourse* jsf, bool no_name)
{
	//jest wartosc lub tekst
	if (childElements.empty() && is_text) jsf->writeToJSFile('"' + Nazwa + '"');
	else{
		if (!no_name) jsf->writeToJSFile('"' + Nazwa + '"' + ":");
		if (childElements.empty()) jsf->writeToJSFile("null");
		else
		{
			XMLnode* child = take_next_child();
			if(!(childElements.size()==1&&childElements.front().second->is_text)) jsf->writeToJSFile("{\n"); //ATT:change
			while (child != NULL)
			{
				if (more_children_have_theSameName(child->Nazwa))
				{
					jsf->writeToJSFile('"' + child->Nazwa + '"'+':');
					jsf->writeToJSFile("[");
					child->writeMe2JSON(jsf, true);
					XMLnode* twin = take_next_child(child->Nazwa);
					while (twin != NULL) //takes all childrens with the same name
					{
						jsf->writeToJSFile(",");
						twin->writeMe2JSON(jsf, true);
						twin = take_next_child(child->Nazwa);
					}

					jsf->writeToJSFile("]"); 
				}
				else child->writeMe2JSON(jsf); 
				
				child = take_next_child();
				if (child!= NULL) jsf->writeToJSFile(",\n");

			} //wszytkie dzieci zostaly obsluzone (conajmniej 1)
			if (!(childElements.size() == 1 && childElements.front().second->is_text)) jsf->writeToJSFile("\n}");
		}
		} //I'm not a leaf 
		return;
}

void XMLnode::setTreeProcessedFalse(XMLnode& node)
{	 
	//if (node != NULL) return;
	for (auto i = node.childElements.begin(); i != node.childElements.end();i++)
	{
		i->first = false;
		setTreeProcessedFalse(*i->second);
	}
}

bool XMLnode::more_children_have_theSameName(string name)
{
	int k = 0;
	for (auto i = childElements.begin(); i != childElements.end(); i++)
	{
		if (i->second->Nazwa == name) k++;
	}
	if(k>1) return true;
	else return false;
}

XMLnode* XMLnode::take_next_child(string name, bool just_check_if_last)
{
	for (auto i = childElements.begin(); i != childElements.end(); i++)
	{
		if (i->first == false)
		{
			if (name == "<")
			{
				if(!just_check_if_last) i->first = true;
				return i->second;
			}
			else if (name == i->second->Nazwa)
			{
				if (!just_check_if_last) i->first = true;
				return i->second;
			}
		}
	}
	return NULL;
}
