#include "layer_transform.hpp"

void layer_transform::rotated_coordinate_to_local(double gx, double gy, double& lx, double& ly) const{
    switch (m_layer_orientation){
    case LO_0  : lx = gx;        ly = gy;        return;
    case LO_90 : lx = gy;        ly = m_h-gx-1.; return;
    case LO_180: lx = m_w-gx-1.; ly = m_h-gy-1.; return;
    case LO_270: lx = m_w-gy-1.; ly = gx;        return;
    }
}

void layer_transform::rotated_coordinate_from_local(double lx, double ly, double& gx, double& gy) const{
    switch (m_layer_orientation){
    case LO_0  : gx = lx;        gy = ly;        return;
    case LO_90 : gx = m_h-ly-1.; gy = lx;        return;
    case LO_180: gx = m_w-lx-1.; gy = m_h-ly-1.; return;
    case LO_270: gx = ly;        gy = m_w-lx-1.; return;
    }
}

void layer_transform::from_local(double lx, double ly, double& gx, double& gy) const
{
    double rx, ry;
    rotated_coordinate_from_local(lx,ly,rx,ry);
    gx = (              rx +m_translationX)/m_zoomFactor;
    gy = (m_coordinates*ry +m_translationY)/m_zoomFactor;
}

void layer_transform::to_local(double gx, double gy, double& lx, double& ly) const
{
    double rx =                m_zoomFactor*gx -m_translationX;
    double ry = m_coordinates*(m_zoomFactor*gy -m_translationY); // should mathematically be a division by m_coordinates, but since it is either 1 or -1, multiplication is fine
    rotated_coordinate_to_local(rx,ry,lx,ly);

    double gx2, gy2;
    from_local(lx, ly, gx2, gy2);
    std::cout << "to_local" << gx << " "<< gy << " "<< lx << " "<< ly <<" "<< gx2 << " "<< gy2 << std::endl;

}

void layer_transform::from_local_int(double lx, double ly, double& gx, double& gy, double delta) const
{
    double rx, ry;
    rotated_coordinate_from_local(lx,ly,rx,ry);
    gx = (              rx +m_translationX+delta-0.5)/m_zoomFactor;
    gy = (m_coordinates*ry +m_translationY+delta-0.5)/m_zoomFactor;
}

void layer_transform::to_local_int(double gx, double gy, double& lx, double& ly, double delta) const
{
    double rx =                m_zoomFactor*gx -m_translationX-delta;
    double ry = m_coordinates*(m_zoomFactor*gy -m_translationY-delta); // should mathematically be a division by m_coordinates, but since it is either 1 or -1, multiplication is fine
    rotated_coordinate_to_local(rx,ry,lx,ly);

    double gx2, gy2;
    from_local(lx, ly, gx2, gy2);
    std::cout << "to_local_int" << gx << " "<< gy << " "<< lx << " "<< ly <<" "<< gx2 << " "<< gy2 << std::endl;
}
