package image_helpers;
import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class RandomWoodGenerator {
    public static void main(String[] args) {
        generate();
    }

    static void generate() {
        BufferedImage image = new BufferedImage(128, 32, BufferedImage.TYPE_INT_RGB);
        
        Color[] colors = {new Color(207, 132, 2), new Color(186, 113, 17), new Color(166, 106, 28), new Color(145, 91, 36), new Color(125, 78, 41)};
        
        // generate the image
        for (int i = 0; i < 100; ++i) {
            for (int color = 0; color < 5; ++color) {
                int x, y, width, height;
                int numRepetitions = 0;
                do {
                    x = (int)(Math.random() * 128);
                    y = (int)(Math.random() * 32);
                    width = (int)(Math.random() * 8) + 5;
                    height = (int)(Math.random() * 3) + 3;
                    ++numRepetitions;
                    // System.out.println("HERE");
                } while (shouldReroll(image, x, y, width, height) && numRepetitions != 1000);
                if (numRepetitions == 1000) {
                    save(image);
                    return;
                }
                for (int currentX = x; currentX != (x + width) % 128;) {
                    for (int currentY = y; currentY != (y + height) % 32;) {
                        image.setRGB(currentX, currentY, colors[color].getRGB());
                        if (++currentY == 32)
                            currentY = 0;
                    }
                    if (++currentX == 128)
                        currentX = 0;
                }
            }
        }

        save(image);
        return;
        
    }
    
    static void save(BufferedImage image) {
        try {
            File outputFile = new File("wooden_floor.png");
            ImageIO.write(image, "png", outputFile);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    static boolean shouldReroll(BufferedImage image, int x, int y, int width, int height) {
        int unmarkedSquares = 0;
        for (int currentX = x; currentX != (x + width) % 128;) {
            for (int currentY = y; currentY != (y + height) % 32;) {
                if (image.getRGB(currentX, currentY) == -16777216)
                    ++unmarkedSquares;
                if (++currentY == 32)
                    currentY = 0;
            }
            if (++currentX == 128)
                currentX = 0;
        }

        return unmarkedSquares < width * height / 4 * 3;
    }
}