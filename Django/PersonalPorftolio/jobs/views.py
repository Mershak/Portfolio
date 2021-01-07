from django.shortcuts import render
from .models import Job


def home(request):
    jobs = Job.objects   #calls all the known Job objects from the database and returns them as a list
    return render(request,'jobs/home.html', {'jobs':jobs})