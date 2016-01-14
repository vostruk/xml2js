#include "XMLnode.h"
#include <iterator>

XMLnode::XMLnode(string name, bool text)
{
	Nazwa = name;
	is_text = text;
}


XMLnode::~XMLnode()
{
}

void XMLnode::drawTree(int level, ostream & out)
{
	out << '\n';
	for (int i = 0; i < level; i++) out << "|     ";
	out << "|____" << Nazwa;
	if (is_text) out << '~';
	for (auto i = childElements.begin(); i != childElements.end(); i++) i->second.drawTree(level + 1, out);
}

XMLnode* XMLnode::createNextChild(string name, bool is_text)
{
	pair<bool, XMLnode> p(false, XMLnode(name, is_text));
	childElements.push_back(p);
	return &childElements.back().second;
}
void XMLnode::writeMe2JSON(JSsourse* jsf, bool no_name)
{

	//jest wartosc lub tekst
	if (childElements.empty() && is_text) jsf->writeToJSFile('"' + Nazwa + '"');
	else{
		if (!no_name)
		{
			//if(childElements.size()>1) jsf->writeToJSFile("{"); //TODO nawias otwierajacy tylko po : a nie po przecinku!!
			jsf->writeToJSFile('"' + Nazwa + '"' + ":");
		}
		if (childElements.empty()) jsf->writeToJSFile("null");
		else
		{
			XMLnode* child = take_next_child();
			if(!(childElements.size()==1&&childElements.front().second.is_text)) jsf->writeToJSFile("{\n"); //ATT:change
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
					//if (take_next_child(Nazwa,true)!=NULL) jsf->writeToJSFile(",");
					//else jsf->writeToJSFile("}");
				}
				else child->writeMe2JSON(jsf); 
				
				child = take_next_child();
				if (child!= NULL) jsf->writeToJSFile(",\n");

			} //wszytkie dzieci zostaly obsluzone (conajmniej 1)
			if (!(childElements.size() == 1 && childElements.front().second.is_text)) jsf->writeToJSFile("\n}");
		}
		//if (!no_name&& childElements.size()>1) jsf->writeToJSFile("}");
		} //I'm not a leaf 
		return;
}

void XMLnode::setTreeProcessedFalse(XMLnode& node)
{	 
	//if (node != NULL) return;
	for (auto i = node.childElements.begin(); i != node.childElements.end();i++)
	{
		i->first = false;
		setTreeProcessedFalse(i->second);
	}
}

bool XMLnode::more_children_have_theSameName(string name)
{
	int k = 0;
	for (auto i = childElements.begin(); i != childElements.end(); i++)
	{
		if (i->second.Nazwa == name) k++;
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
				return &i->second;
			}
			else if (name == i->second.Nazwa)
			{
				if (!just_check_if_last) i->first = true;
				return &i->second;
			}
		}
	}
	return NULL;
}
