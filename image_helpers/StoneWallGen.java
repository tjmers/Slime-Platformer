package image_helpers;
import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

// this is mostly just a copy paste from StoneGen.java

import javax.imageio.ImageIO;

public class StoneWallGen {
    public static void main(String[] args) {
        generate();
    }

    static void generate() {
        BufferedImage image = new BufferedImage(32, 32, BufferedImage.TYPE_INT_RGB);
        
        Color[] colors = {new Color(17, 17, 17, 200), new Color(15, 15, 15, 200), new Color(20, 20, 20, 200), new Color(25, 25, 25, 200)};
        
        // generate the image
        for (int i = 0; i < image.getWidth(); ++i) {
            for (int j = 0; j < image.getHeight(); ++j) {
                image.setRGB(i, j, colors[(int)(Math.random() * colors.length)].getRGB());
            }
        }

        save(image);
        return;
        
    }
    
    static void save(BufferedImage image) {
        try {
            File outputFile = new File("stone_wall.png");
            ImageIO.write(image, "png", outputFile);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

}