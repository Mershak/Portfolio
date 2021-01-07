from django.db import models  #make sure to add any new app to settings APPS[]

class Job(models.Model):  #creates a Job object instance model, a class in coding terms

    image = models.ImageField(upload_to='images/')  #create an attribute that is an uploaded image
                                                    #may need to download 'Pillow' for imagefield, use
                                                    #'python -m pip install --upgrade Pillow' command

    summary = models.CharField(max_length=200)