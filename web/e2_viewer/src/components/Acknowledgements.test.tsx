import { describe, it, expect } from 'vitest'
import { render, screen, fireEvent } from '@testing-library/react'
import { Acknowledgements } from './Acknowledgements'
import "@testing-library/jest-dom";

/**
 * @vitest-environment jsdom
 */


describe('Acknowledgements', () => {
    it('renders all technology logos with correct links', () => {
        render(<Acknowledgements />)
        
        // Check for Vite link and logo
        const viteLink = screen.getByRole('link', { name: /vite logo/i })
        expect(viteLink).toHaveAttribute('href', 'https://vite.dev')
        
        // Check for React link and logo
        const reactLink = screen.getByRole('link', { name: /react logo/i })
        expect(reactLink).toHaveAttribute('href', 'https://react.dev')
        
        // Check for Tailwind link and logo
        const tailwindLink = screen.getByRole('link', { name: /tailwind css logo/i })
        expect(tailwindLink).toHaveAttribute('href', 'https://tailwindcss.com')
        
        // Check for daisyUI link and logo
        const daisyuiLink = screen.getByRole('link', { name: /daisyui logo/i })
        expect(daisyuiLink).toHaveAttribute('href', 'https://daisyui.com')
        
        // Check for three.js link
        const threejsLinks = screen.getAllByRole('link', { name: /three\.js logo/i })
        expect(threejsLinks[0]).toHaveAttribute('href', 'https://threejs.org/')
        expect(threejsLinks[1]).toHaveAttribute('href', 'https://github.com/pmndrs/react-three-fiber')
        expect(threejsLinks[2]).toHaveAttribute('href', 'https://github.com/pmndrs/drei')
    })

    it('opens links in new tab', () => {
        render(<Acknowledgements />)
        
        const links = screen.getAllByRole('link')
        links.forEach(link => {
            expect(link).toHaveAttribute('target', '_blank')
        })
    })

    it('displays instructional text', () => {
        render(<Acknowledgements />)
        
        expect(screen.getByText('Click on the logos above to learn more about the technologies used in this application.')).toBeInTheDocument()
    })

    it('increments counter when button is clicked', () => {
        render(<Acknowledgements />)
        
        const button = screen.getByRole('button', { name: /does react react\? 0/i })
        expect(button).toBeInTheDocument()
        
        fireEvent.click(button)
        expect(screen.getByRole('button', { name: /does react react\? 1/i })).toBeInTheDocument()
        
        fireEvent.click(button)
        expect(screen.getByRole('button', { name: /does react react\? 2/i })).toBeInTheDocument()
    })

    it('starts with counter at 0', () => {
        render(<Acknowledgements />)
        
        expect(screen.getByRole('button', { name: /does react react\? 0/i })).toBeInTheDocument()
    })
})