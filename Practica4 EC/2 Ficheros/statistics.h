//-----------------------------------------------------------------------------
// statistics.h
//-----------------------------------------------------------------------------

#ifndef statistics_h
#define statistics_h

//-----------------------------------------------------------------------------

#include <algorithm> // nth_element
#include <cmath>     // 
#include <stdexcept> // invalid_argument
#include <iostream>  // ostream
#include <limits>    // numeric_limits
#include <map>       // map
#include <numeric>   //
#include <ostream>   // ostream 
#include <vector>    // vector

//-----------------------------------------------------------------------------

namespace ec
{
  //---------------------------------------------------------------------------
  // covariance
  //---------------------------------------------------------------------------
  
  template <typename _iterator1, typename _iterator2> 
  double sum_of_squares(_iterator1 __first1, _iterator1 __last1, 
		      _iterator2 __first2, _iterator2 __last2, 
		      double __mean1, double __mean2);
  
  template <typename _iterator1, typename _iterator2> 
  double covariance(_iterator1 __first1, _iterator1 __last1, 
		  _iterator2 __first2, _iterator2 __last2, 
		  double __mean1, double __mean2)
  {
    return sum_of_squares(__first1, __last1, 
			  __first2, __last2, 
			  __mean1, __mean2) / 
      (__last1 - __first1);
  }
  
  template <typename _iterator1, typename _iterator2> 
  double covariance(_iterator1 __first1, _iterator1 __last1, 
		  _iterator2 __first2, _iterator2 __last2)
  {
    return covariance(__first1, __last1, __first2, __last2, 
		      mean(__first1, __last1), 
		      mean(__first2, __last2));
  }
   
  //---------------------------------------------------------------------------
  // histogram
  //---------------------------------------------------------------------------

  typedef std::map<double, unsigned> histogram_data;

  template <class _iterator> 
  histogram_data histogram(_iterator first, _iterator last)
  {
    histogram_data data;
    for (; first != last; ++first)
      ++data[*first];
    return data;
  }
  
  //---------------------------------------------------------------------------
  // max
  //---------------------------------------------------------------------------
  
  template <class _iterator> 
  double max(_iterator first, _iterator last)
  {
    return *std::max_element(first, last);
  }

  //---------------------------------------------------------------------------
  // min
  //---------------------------------------------------------------------------

  template <class _iterator> 
  double min(_iterator first, _iterator last)
  {
    return *std::min_element(first, last);
  }

  //---------------------------------------------------------------------------
  // mean
  //---------------------------------------------------------------------------

  template <class _iterator> 
  double mean(_iterator first, _iterator last)
  {
    return std::accumulate(first, last, 0.0) / (last - first);
  }
  
  //---------------------------------------------------------------------------
  // median
  //---------------------------------------------------------------------------

  template <class _iterator> double median(_iterator first, _iterator last)
  {
    _iterator center = first + (last - first) / 2;
    std::nth_element(first, center, last);
    return *center;
  }

  //---------------------------------------------------------------------------
  // mode
  //---------------------------------------------------------------------------
  
  histogram_data mode(histogram_data data)
  {
    for (histogram_data::iterator i = data.begin(); i != data.end(); ++i)
      for (histogram_data::iterator j = data.begin(); j != data.end(); ++j)
  	if (i->second < j->second)
  	  data.erase(i->first);
    return data;
  }

  template <class _iterator> 
  histogram_data mode(_iterator first, _iterator last)
  {
    return mode(histogram(first, last));
  }

  //---------------------------------------------------------------------------
  // variance
  //---------------------------------------------------------------------------

  template <class _iterator> 
  double sum_of_squares(_iterator first, _iterator last);

  template <class _iterator> 
  double sum_of_squares(_iterator first, _iterator last, double _mean);

  template <class _iterator> 
  double variance(_iterator first, _iterator last, double _mean)
  {
    return sum_of_squares(first, last, _mean) / ((last - first) - 1.0);
  }
  
  template <class _iterator> 
  double variance(_iterator first, _iterator last)
  {
    return variance(first, last, mean(first, last));
  }

  //---------------------------------------------------------------------------
  // stddev
  //---------------------------------------------------------------------------
  
  template <class _iterator> 
  double stddev(_iterator __first, _iterator __last, double __mean)
  {
    return sqrt(variance(__first, __last, __mean));
  } 

  template <class _iterator> 
  double stddev(_iterator __first, _iterator __last)
  {
    return sqrt(variance(__first, __last));
  }
  
  //---------------------------------------------------------------------------
  // statistics
  //---------------------------------------------------------------------------
  
  class statistics
  {
  public:
    template <class _iterator> 
    explicit statistics(_iterator first, _iterator last)
    {
      operator()(first, last);
    }
    
    template <class _iterator>
    void operator()(_iterator first, _iterator last)
    {
      max_    = ec::max (first, last);
      min_    = ec::min (first, last);
      mean_   = ec::mean(first, last);
      stddev_ = ec::stddev(first, last, mean_);
    }
    
    double max()    const { return max_;    }
    double min()    const { return min_;    }
    double mean()   const { return mean_;   }
    double stddev() const { return stddev_; }

    friend std::ostream& operator<<(std::ostream& os, const statistics& s)
    {
      return os << s.max() << " "
		<< s.mean() << " "
		<< s.min() << " "
		<< s.stddev();
    }
    
  private:
    double max_, min_, mean_, stddev_;
  };
 
  //---------------------------------------------------------------------------
  // sum_of_squares
  //---------------------------------------------------------------------------
  
  template <class _iterator> 
  double sum_of_squares(_iterator first, _iterator last, double _mean)
  {
    double sum = 0;
    
    for (; first != last; ++first)
      sum += (*first - _mean) * (*first - _mean);     
    
    return sum;
  }
  
  template <class _iterator>
  double sum_of_squares(_iterator first, _iterator last)
  {
    return sum_of_squares(first, last, mean(first, last));
  }
  
  template <typename _iterator1, typename _iterator2> 
  double sum_of_squares(_iterator1 __first1, _iterator1 __last1, 
		      _iterator2 __first2, _iterator2 __last2, 
		      double __mean1, double __mean2)
  {
    double r = 0;
      
    for (; __first1 != __last1; ++__first1, ++__first2)
      r += (*__first1 - __mean1) * (*__first2 - __mean2);     
      
    return r;
  }
  
  template <typename _iterator1, typename _iterator2> 
  double sum_of_squares(_iterator1 __first1, _iterator1 __last1, 
		      _iterator2 __first2, _iterator2 __last2)
  {
    return sum_of_squares(__first1, __last1, 
			  __first2, __last2, 
			  mean(__first1, __last1), 
			  mean(__first2, __last2));
  }

  //---------------------------------------------------------------------------

} // namespace ec

//-----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const ec::histogram_data& h)
{
  for (ec::histogram_data::const_iterator i = h.begin(); i != h.end(); ++i)
    os << "<" << i->first << "," << i->second << ">";
  return os;
}

//-----------------------------------------------------------------------------

#endif // statistics_h

// Local Variables: 
// mode:C++ 
// End:
