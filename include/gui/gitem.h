#pragma once

#include <iostream>
#include <string>

namespace e2 {

    class GItemHandle {
        public:
            GItemHandle() {}
            virtual ~GItemHandle() {}
            bool needsUpdate() const { return m_needsUpdate; }
            void setNeedsUpdate(bool needsUpdate) { m_needsUpdate = needsUpdate; }  
        private:
            bool m_needsUpdate = true; // whether the graphical item needs to be updated in the graphics framework
    };

    class GItem {
        public:
            GItem() {}
            virtual ~GItem() {
                delete m_handle;
            };
            GItemHandle* handle() { return m_handle; }
            void setHandle(GItemHandle* handle) { m_handle = handle; }
            virtual void print(std::ostream& os) const = 0;
            friend std::ostream& operator<<(std::ostream& os, const GItem& m);
        private:
            //std::string m_name;
            //GAppearance* m_appearance;
            GItemHandle* m_handle = nullptr;
    };

    class GBlock : public GItem {
        public:
            GBlock(double width, double height, double depth) : GItem(), m_width(width), m_height(height), m_depth(depth) {}
            void print(std::ostream& os) const override;
            double width() const { return m_width; }
            double height() const { return m_height; }
            double depth() const { return m_depth; }
        private:
            double m_width;
            double m_height;
            double m_depth;
    };

};

