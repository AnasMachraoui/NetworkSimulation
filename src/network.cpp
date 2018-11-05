#include "network.h"
#include "random.h"


void Network::resize(const size_t &n)
{
	if (values.size()<n) { while (values.size()<n) {values.push_back(0.0);} }
	else {	while (values.size()>n) {values.pop_back();} }
	RNG.normal(values,0,1);
}

bool Network::add_link(const size_t &a, const size_t &b)
{
     if (a==b) return false; //if the node is the same then don't add link
	 if ((values.size()-1<a) or (values.size()-1<b)) return false;//check if node exists
	 if (not(links.empty()))
	 {
		 for (std::multimap<size_t,size_t>::iterator it=links.begin(); it!=links.end(); ++it)
		{
			if (((it->first==a) and (it->second==b)) or ((it->first==b) and (it->second==a))) return false;//if link already exists then don't add
		}
	 }
	 links.insert(std::pair<size_t,size_t>(a,b));
	 links.insert(std::pair<size_t,size_t>(b,a)); //link is bidirectional
	 return true;
}

size_t Network::random_connect(const double &mean_deg)
{
	
	links.clear();
	size_t total_links(0);
	std::vector<size_t> nodes(values.size());
	size_t max_deg;
	for (size_t i(0); i<nodes.size(); ++i)
	{	
		nodes[i]=i;
	}	
	for (size_t i(0); i<nodes.size(); ++i)
	{	
		RNG.shuffle(nodes); //in order to get random links
		max_deg=RNG.poisson(mean_deg);
		size_t j=0;
		size_t degree=0;
		while( (degree<max_deg) and (j<nodes.size()))
			{
				if (add_link(i,nodes[j])) {++degree;++total_links;}
				++j;
			}
	} 
	
	return total_links;
}
 
size_t Network::set_values(const std::vector<double> &vec)
{
	size_t success(0);
	if (not(vec.empty()))
	{
			if (vec.size()>=values.size()) 
			{
				for (size_t i(0);i<=values.size()-1;i++) {values[i]=vec[i]; ++success;}
			}
			else 
			{
				for (size_t i(0);i<=vec.size()-1;i++) {values[i]=vec[i];++success;}
			}
	}
	return success;
}

size_t Network::size() const
{
	return values.size();
}
  
size_t Network::degree(const size_t &_n) const
{
	if ((not(values.empty())) and (not(links.empty())))
		{
			size_t deg(0);
			for (std::multimap<size_t,size_t>::const_iterator it=links.begin(); it!=links.end(); ++it)
			{
				if (it->first==_n) ++deg;
			}
			return deg;
		}
	return 0;
}
 
double Network::value(const size_t &_n) const
{
	return values[_n];
}
 
std::vector<double> Network::sorted_values() const
{	
	
	std::vector<double> sorted(values);
	double temp;
	for (size_t i(0); i<sorted.size(); ++i)
	{
		int max(i);
		for (size_t j(i); j<sorted.size(); ++j)
		{
			if (sorted[j]>sorted[max]) max = j;
		}
	temp=sorted[i];
	sorted[i]=sorted[max];
	sorted[max]=temp;
	}
	return sorted; 		
		
}
std::vector<size_t> Network::neighbors(const size_t &n) const
{
	  std::vector<size_t> neighbours; 
	  if (not(links.empty()))
	  {
		for (std::multimap<size_t,size_t>::const_iterator it=links.begin(); it!=links.end(); ++it)
		{
			if (it->first==n) neighbours.push_back(it->second); 
		}
	  }
	 return neighbours;
}

