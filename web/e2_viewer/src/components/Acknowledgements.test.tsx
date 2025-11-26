import { describe, it, expect } from 'vitest'
import { render, screen } from '@testing-library/react'
import { Acknowledgements } from './Acknowledgements'
import "@testing-library/jest-dom";

/**
 * @vitest-environment jsdom
 */


describe('Acknowledgements', () => {
    it('renders all technology logos with correct text and links', () => {
        render(<Acknowledgements />)

        // Check the caption text
        expect(screen.getByText('Acknowledgements:')).toBeInTheDocument()
        
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
    })

    it('opens links in new tab', () => {
        render(<Acknowledgements />)
        
        const links = screen.getAllByRole('link')
        links.forEach(link => {
            expect(link).toHaveAttribute('target', '_blank')
        })
    })

})