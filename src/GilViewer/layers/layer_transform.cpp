#include "layer_transform.hpp"

wxRealPoint layer_transform::rotated_coordinate_to_local(const wxRealPoint& pt)const{
        wxRealPoint out(pt);
        if(m_h==0 && m_w==0)
            return out;
        switch (m_layer_orientation){
        case LO_0: break;//return pt;//correct
        case LO_90: out= wxRealPoint(pt.y,m_h-pt.x-1);break;//correct
        case LO_180: out= wxRealPoint(m_w-pt.x-1.,m_h-pt.y-1.);break;//correct
        case LO_270: out= wxRealPoint(m_w-pt.y-1,pt.x);break;//correct
        }
        std::cout<<__FUNCTION__<<" "<<pt.x<<" "<<pt.y<<" "<<out.x<<" "<<out.y<<" "<<m_w<<" "<<m_h<<" "<<(unsigned int)m_layer_orientation<<std::endl;
        //return pt;
        return out;
    }

wxRealPoint  layer_transform::rotated_coordinate_from_local(const wxRealPoint& pt)const{
    wxRealPoint out(pt);
//        return out;
        if(m_h==0 && m_w==0)
            return out;
        switch (m_layer_orientation){
        case LO_0: break;//return pt;//correct
        case LO_90: out=wxRealPoint(m_h-pt.y-1,pt.x);break;//return wxRealPoint(m_h-pt.y-1,pt.x);//correct
        case LO_180: out=wxRealPoint(m_w-pt.x-1.,m_h-pt.y-1.);break;//return wxRealPoint(m_w-pt.x-1.,m_h-pt.y-1.);//correct
        case LO_270: out=wxRealPoint(pt.y,m_w-pt.x-1.);break;//return wxRealPoint(pt.y,m_w-pt.x-1.);//correct
        }
        std::cout<<__FUNCTION__<<" "<<pt.x<<" "<<pt.y<<" "<<out.x<<" "<<out.y<<" "<<m_w<<" "<<m_h<<" "<<(unsigned int)m_layer_orientation<<std::endl;
        return out;
    }
